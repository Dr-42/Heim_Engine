#ifndef HEIM_SPRITE_H
#define HEIM_SPRITE_H

#include <GL/glew.h>

#include "math/heim_mat.h"
#include "math/heim_vec.h"
#include "renderer/heim_shader.h"
#include "renderer/heim_texture.h"

typedef struct Vertex {
    HeimVec3f position;
    HeimVec2f tex_coords;
} Vertex;

typedef struct HeimSprite {
    HeimVec4f color;
    HeimTexture* texture;

    Vertex vertices[4];

    HeimMat4 model;
    HeimMat4 view;
    HeimMat4 projection;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} HeimSprite;

HeimSprite* heim_create_sprite(HeimTexture* texture);
void heim_sprite_free(HeimSprite* sprite);
void heim_sprite_render(HeimSprite* sprite);

#endif  // HEIM_SPRITE_H
