#include "renderer/heim_sprite.h"

#include "core/heim_memory.h"
#include "math/heim_mat.h"

HeimSprite* heim_create_sprite(HeimTexture* texture) {
    HeimSprite* sprite = HEIM_MALLOC(HeimSprite, HEIM_MEMORY_TYPE_RENDERER);
    sprite->texture = texture;

    sprite->vertices[0] = (Vertex){(HeimVec3f){1.0f, 1.0f, 0.0f}, (HeimVec2f){1.0f, 1.0f}};

    sprite->vertices[1] = (Vertex){(HeimVec3f){1.0f, 0.0f, 0.0f}, (HeimVec2f){1.0f, 0.0f}};

    sprite->vertices[2] = (Vertex){(HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec2f){0.0f, 0.0f}};

    sprite->vertices[3] = (Vertex){(HeimVec3f){0.0f, 1.0f, 0.0f}, (HeimVec2f){0.0f, 1.0f}};

    uint16_t indices[] = {0, 3, 1, 1, 3, 2};

    sprite->model = heim_mat4_identity();
    sprite->view = heim_mat4_identity();
    sprite->projection = heim_mat4_identity();

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

    return sprite;
}

void heim_sprite_free(HeimSprite* sprite) {
    glDeleteVertexArrays(1, &sprite->vao);
    glDeleteBuffers(1, &sprite->vbo);
    glDeleteBuffers(1, &sprite->ebo);
    HEIM_FREE(sprite, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_sprite_render(HeimSprite* sprite) {
    GLuint vao = sprite->vao;
    GLuint vbo = sprite->vbo;
    GLuint ebo = sprite->ebo;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
}
