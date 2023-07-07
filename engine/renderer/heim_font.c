#include "renderer/heim_font.h"

#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include <stdint.h>

#include "core/heim_logger.h"
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
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

        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        HeimCharacter character = {
            .texture_id = texture,
            .size = (HeimVec2f){face->glyph->bitmap.width, face->glyph->bitmap.rows},
            .bearing = (HeimVec2f){face->glyph->bitmap_left, face->glyph->bitmap_top},
            .advance = face->glyph->advance.x,
        };

        font->characters[i] = character;
    }

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

    for (char* c = text; *c; c++) {
        HeimCharacter ch = font->characters[*c];

        float xpos = position.x + ch.bearing.x * scale;
        float ypos = position.y + (font->characters['H'].bearing.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            {xpos, ypos, 0.0f, 0.0f},
            {xpos, ypos + h, 0.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 1.0f},

            {xpos, ypos, 0.0f, 0.0f},
            {xpos + w, ypos + h, 1.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 0.0f},
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        position.x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
