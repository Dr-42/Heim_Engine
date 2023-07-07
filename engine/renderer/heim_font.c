#include "renderer/heim_font.h"

#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include <stdint.h>

#include "core/heim_logger.h"
#include "core/utils/heim_vector.h"
#include "math/heim_math_common.h"
#include FT_FREETYPE_H

#include "core/heim_memory.h"

HeimFont* heim_font_create(HeimVec2f size) {
    HeimFont* font = HEIM_MALLOC(HeimFont, HEIM_MEMORY_TYPE_RENDERER);
    HeimShader* shader = heim_shader_create();
    heim_shader_init(shader, "assets/shaders/text.vert", "assets/shaders/text.frag");
    font->shader = shader;
    font->size = size;
    glGenVertexArrays(1, &font->vao);
    glGenBuffers(1, &font->vbo);
    glBindVertexArray(font->vao);
    glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return font;
}

void heim_font_free(HeimFont* font) {
    heim_shader_free(font->shader);
    HEIM_FREE(font, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_font_init(HeimFont* font, char* path, uint32_t size) {
    FT_Library ft;
    int atlas_width = 0;
    int atlas_height = 0;

    if (FT_Init_FreeType(&ft)) {
        HEIM_LOG_ERROR("Failed to initialize FreeType library");
    }
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        HEIM_LOG_ERROR("Failed to load font");
        HEIM_LOG_ERROR("%s", path);
    }

    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (uint8_t i = 0; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            HEIM_LOG_ERROR("Failed to load glyph");
            continue;
        }
        atlas_width += face->glyph->bitmap.width;
        atlas_height = heim_math_max(atlas_height, face->glyph->bitmap.rows);
    }

    font->atlas_width = atlas_width;

    uint32_t atlas;
    glGenTextures(1, &atlas);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        atlas_width,
        atlas_height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        NULL);

    int x = 0;
    for (uint8_t i = 0; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            HEIM_LOG_ERROR("Failed to load glyph");
            continue;
        }
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            x,
            0,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        HeimCharacter character = {
            .texture_id = atlas,
            .size = (HeimVec2f){face->glyph->bitmap.width, face->glyph->bitmap.rows},
            .bearing = (HeimVec2f){face->glyph->bitmap_left, face->glyph->bitmap_top},
            .advance = face->glyph->advance.x,
            .tex_coords_start = (HeimVec2f){x / (float)atlas_width, 0},
            .tex_coords_end = (HeimVec2f){(x + face->glyph->bitmap.width) / (float)atlas_width, face->glyph->bitmap.rows / (float)atlas_height}};

        font->characters[i] = character;
        x += face->glyph->bitmap.width;
    }
    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void heim_font_render_text(HeimFont* font, char* text, HeimVec2f position, float scale, HeimVec3f color) {
    heim_shader_bind(font->shader);
    heim_shader_set_uniform3f(font->shader, "textColor", color);
    heim_shader_set_uniform_mat4(font->shader, "projection", heim_mat4_ortho(0.0f, font->size.x, font->size.y, 0.0f, -0.9f, 1.0f));
    heim_shader_set_uniform1i(font->shader, "text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->vao);

    float* verts = heim_vector_create(float);
    for (uint8_t* c = (uint8_t*)text; *c; c++) {
        HeimCharacter ch = font->characters[*c];

        float xpos = position.x + ch.bearing.x * scale;
        float ypos = position.y + (font->characters['H'].bearing.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            {xpos, ypos + h, ch.tex_coords_start.x, ch.tex_coords_end.y},
            {xpos, ypos, ch.tex_coords_start.x, ch.tex_coords_start.y},
            {xpos + w, ypos, ch.tex_coords_end.x, ch.tex_coords_start.y},

            {xpos, ypos + h, ch.tex_coords_start.x, ch.tex_coords_end.y},
            {xpos + w, ypos, ch.tex_coords_end.x, ch.tex_coords_start.y},
            {xpos + w, ypos + h, ch.tex_coords_end.x, ch.tex_coords_end.y}};

        position.x += (ch.advance >> 6) * scale;

        for (uint8_t i = 0; i < 6; i++) {
            heim_vector_push(verts, vertices[i][0]);
            heim_vector_push(verts, vertices[i][1]);
            heim_vector_push(verts, vertices[i][2]);
            heim_vector_push(verts, vertices[i][3]);
        }
    }
    glBindTexture(GL_TEXTURE_2D, font->characters[0].texture_id);
    glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
    glBufferData(GL_ARRAY_BUFFER, heim_vector_length(verts) * 4, verts, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, heim_vector_length(verts));
    heim_vector_destroy(verts);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
