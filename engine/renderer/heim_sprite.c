#include "renderer/heim_sprite.h"

#define STB_IMAGE_IMPLEMENTATION
#include <cglm/cglm.h>
#include <stb/stb_image.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "math/heim_mat.h"
#include "math/heim_math_common.h"

typedef struct Vertex {
    HeimVec3f position;
    HeimVec2f tex_coords;
} Vertex;

typedef struct HeimSprite {
    HeimVec2f position;
    HeimVec2f size;
    HeimVec4f color;
    float rotation;
    char* texture_path;
    GLuint texture;

    Vertex vertices[4];

    HeimMat4 model;
    HeimMat4 view;
    HeimMat4 projection;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} HeimSprite;

void _update_sprite_mats(HeimSprite* sprite) {
    sprite->size.y *= -1;
    sprite->model = heim_mat4_identity();
    sprite->view = heim_mat4_identity();

    sprite->model = heim_mat4_translate(heim_mat4_identity(), (HeimVec3f){sprite->position.x, sprite->position.y, 0.0f});
    sprite->model = heim_mat4_rotate(sprite->model, sprite->rotation, (HeimVec3f){0.0f, 0.0f, 1.0f});
    sprite->model = heim_mat4_translate(sprite->model, (HeimVec3f){-0.5 * sprite->size.x, -0.5 * sprite->size.y, 0.0f});
    sprite->model = heim_mat4_scale(sprite->model, (HeimVec3f){sprite->size.x, sprite->size.y, 1.0f});

    sprite->view = heim_mat4_translate(sprite->view, (HeimVec3f){0.0f, 0.0f, -1.0f});

    // NOTE: Projection set in heim_renderer.c
    sprite->size.y *= -1;
}

HeimSprite* heim_create_sprite(char* texture_path) {
    HeimSprite* sprite = HEIM_MALLOC(HeimSprite, HEIM_MEMORY_TYPE_RENDERER);
    sprite->position = (HeimVec2f){0.0f, 0.0f};
    sprite->size = (HeimVec2f){0.5f, 0.5f};
    sprite->color = (HeimVec4f){1.0f, 1.0f, 1.0f, 1.0f};
    sprite->rotation = 0.0f;
    sprite->texture_path = texture_path;

    int w, h, channels;
    stbi_set_flip_vertically_on_load(1);
    uint8_t* texture_data = stbi_load(texture_path, &w, &h, &channels, 0);

    if (!texture_data) {
        HEIM_LOG_ERROR("Failed to load texture: %s", texture_path);
    }

    sprite->vertices[0] = (Vertex){
        (HeimVec3f){1.0f, 1.0f, 0.0f},
        (HeimVec2f){1.0f, 1.0f}};

    sprite->vertices[1] = (Vertex){
        (HeimVec3f){1.0f, 0.0f, 0.0f},
        (HeimVec2f){1.0f, 0.0f}};

    sprite->vertices[2] = (Vertex){
        (HeimVec3f){0.0f, 0.0f, 0.0f},
        (HeimVec2f){0.0f, 0.0f}};

    sprite->vertices[3] = (Vertex){
        (HeimVec3f){0.0f, 1.0f, 0.0f},
        (HeimVec2f){0.0f, 1.0f}};

    uint16_t indices[] = {0, 3, 1, 1, 3, 2};

    sprite->model = heim_mat4_identity();
    sprite->view = heim_mat4_identity();
    sprite->projection = heim_mat4_identity();

    sprite->rotation = 0.0f;

    glGenVertexArrays(1, &sprite->vao);
    glGenBuffers(1, &sprite->vbo);
    glGenBuffers(1, &sprite->ebo);

    glBindVertexArray(sprite->vao);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprite->vertices), sprite->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &sprite->texture);
    glBindTexture(GL_TEXTURE_2D, sprite->texture);

    if (channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    } else if (channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    } else {
        HEIM_LOG_ERROR("Unsupported number of channels: %d", channels);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texture_data);

    _update_sprite_mats(sprite);

    return sprite;
}

void heim_sprite_free(HeimSprite* sprite) {
    glDeleteVertexArrays(1, &sprite->vao);
    glDeleteBuffers(1, &sprite->vbo);
    glDeleteBuffers(1, &sprite->ebo);
    HEIM_FREE(sprite, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_sprite_set_position(HeimSprite* sprite, HeimVec2f position) {
    sprite->position = position;

    _update_sprite_mats(sprite);
}

void heim_sprite_set_size(HeimSprite* sprite, HeimVec2f size) {
    sprite->size = size;
    _update_sprite_mats(sprite);
}

void heim_sprite_set_color(HeimSprite* sprite, HeimVec4f color) {
    sprite->color = color;
}

void heim_sprite_set_rotation(HeimSprite* sprite, float degrees) {
    sprite->rotation = degrees;
    _update_sprite_mats(sprite);
}
void heim_sprite_set_projection(HeimSprite* sprite, HeimMat4 projection) {
    sprite->projection = projection;
    _update_sprite_mats(sprite);
}

HeimVec2f heim_sprite_get_position(HeimSprite* sprite) {
    return sprite->position;
}

HeimVec2f heim_sprite_get_size(HeimSprite* sprite) {
    return sprite->size;
}

HeimVec4f heim_sprite_get_color(HeimSprite* sprite) {
    return sprite->color;
}

float heim_sprite_get_rotation(HeimSprite* sprite) {
    return sprite->rotation;
}

GLuint heim_sprite_get_texture(HeimSprite* sprite) {
    return sprite->texture;
}

GLuint heim_sprite_get_vao(HeimSprite* sprite) {
    return sprite->vao;
}

GLuint heim_sprite_get_vbo(HeimSprite* sprite) {
    return sprite->vbo;
}

GLuint heim_sprite_get_ebo(HeimSprite* sprite) {
    return sprite->ebo;
}

HeimMat4 heim_sprite_get_model(HeimSprite* sprite) {
    return sprite->model;
}

HeimMat4 heim_sprite_get_view(HeimSprite* sprite) {
    return sprite->view;
}

HeimMat4 heim_sprite_get_projection(HeimSprite* sprite) {
    return sprite->projection;
}
