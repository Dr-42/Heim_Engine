#include "renderer/heim_sprite.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "core/heim_memory.h"
#include "core/heim_logger.h"
#include "math/heim_vector.h"

typedef struct HeimSprite {
    HeimVec2f position;
    HeimVec2f size;
    HeimVec4f color;
    char* texture_path;
    void* texture_data;
    
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} HeimSprite;

HeimSprite* heim_create_sprite(char* texture_path){
    HeimSprite* sprite = HEIM_MALLOC(HeimSprite, HEIM_MEMORY_TYPE_RENDERER);
    sprite->position = (HeimVec2f){0.0f, 0.0f};
    sprite->size = (HeimVec2f){1.0f, 1.0f};
    sprite->color = (HeimVec4f){1.0f, 1.0f, 1.0f, 1.0f};
    sprite->texture_path = texture_path;

    int w, h, channels;
    sprite->texture_data = stbi_load(texture_path, &w, &h, &channels, 0);

    if(!sprite->texture_data){
        HEIM_LOG_ERROR("Failed to load texture: %s", texture_path);
    }

    HeimVec3f vertices[] = {
        (HeimVec3f){-1.f, -1.f, 0.0f},
        (HeimVec3f){1.f, -1.f, 0.0f},
        (HeimVec3f){1.f, 1.f, 0.0f},
        (HeimVec3f){-1.f, 1.f, 0.0f}};

    uint16_t indices[] = {0, 1, 2, 2, 3, 0};

    glGenVertexArrays(1, &sprite->vao);
    glGenBuffers(1, &sprite->vbo);
    glGenBuffers(1, &sprite->ebo);

    glBindVertexArray(sprite->vao);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return sprite;
}

void heim_sprite_free(HeimSprite* sprite){
    stbi_image_free(sprite->texture_data);
    
    glDeleteVertexArrays(1, &sprite->vao);
    glDeleteBuffers(1, &sprite->vbo);
    glDeleteBuffers(1, &sprite->ebo);
    HEIM_FREE(sprite, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_sprite_set_position(HeimSprite* sprite, HeimVec2f position){
    sprite->position = position;

    glBindVertexArray(sprite->vao);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);

    //Take size and position into account
    HeimVec3f vertices[] = {
        (HeimVec3f){-1.f * sprite->size.x + sprite->position.x, -1.f * sprite->size.y + sprite->position.y, 0.0f},
        (HeimVec3f){1.f * sprite->size.x + sprite->position.x, -1.f * sprite->size.y + sprite->position.y, 0.0f},
        (HeimVec3f){1.f * sprite->size.x + sprite->position.x, 1.f * sprite->size.y + sprite->position.y, 0.0f},
        (HeimVec3f){-1.f * sprite->size.x + sprite->position.x, 1.f * sprite->size.y + sprite->position.y, 0.0f}};
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void heim_sprite_set_size(HeimSprite* sprite, HeimVec2f size){
    sprite->size = size;

    glBindVertexArray(sprite->vao);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);

    //Take size and position into account
    HeimVec3f vertices[] = {
        (HeimVec3f){-1.f * sprite->size.x + sprite->position.x, -1.f * sprite->size.y + sprite->position.y, 0.0f},
        (HeimVec3f){1.f * sprite->size.x + sprite->position.x, -1.f * sprite->size.y + sprite->position.y, 0.0f},
        (HeimVec3f){1.f * sprite->size.x + sprite->position.x, 1.f * sprite->size.y + sprite->position.y, 0.0f},
        (HeimVec3f){-1.f * sprite->size.x + sprite->position.x, 1.f * sprite->size.y + sprite->position.y, 0.0f}};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void heim_sprite_set_color(HeimSprite* sprite, HeimVec4f color){
    sprite->color = color;
}

HeimVec2f heim_sprite_get_position(HeimSprite* sprite){
    return sprite->position;
}

HeimVec2f heim_sprite_get_size(HeimSprite* sprite){
    return sprite->size;
}

HeimVec4f heim_sprite_get_color(HeimSprite* sprite){
    return sprite->color;
}

void* heim_sprite_get_texture_data(HeimSprite* sprite){
    return sprite->texture_data;
}

GLuint heim_sprite_get_vao(HeimSprite* sprite){
    return sprite->vao;
}

GLuint heim_sprite_get_vbo(HeimSprite* sprite){
    return sprite->vbo;
}

GLuint heim_sprite_get_ebo(HeimSprite* sprite){
    return sprite->ebo;
}
