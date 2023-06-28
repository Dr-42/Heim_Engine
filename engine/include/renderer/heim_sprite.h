#ifndef HEIM_SPRITE_H
#define HEIM_SPRITE_H

#include <GL/glew.h>
#include <cglm/cglm.h>

#include "math/heim_mat.h"
#include "math/heim_vec.h"
#include "renderer/heim_texture.h"

typedef struct HeimSprite HeimSprite;

HeimSprite* heim_create_sprite(HeimTexture* texture);
void heim_sprite_set_position(HeimSprite* sprite, HeimVec2f position);
void heim_sprite_set_size(HeimSprite* sprite, HeimVec2f size);
void heim_sprite_set_color(HeimSprite* sprite, HeimVec4f color);
void heim_sprite_set_rotation(HeimSprite* sprite, float degrees);
void heim_sprite_set_projection(HeimSprite* sprite, HeimMat4 projection);

HeimVec2f heim_sprite_get_position(HeimSprite* sprite);
HeimVec2f heim_sprite_get_size(HeimSprite* sprite);
HeimVec4f heim_sprite_get_color(HeimSprite* sprite);
HeimTexture* heim_sprite_get_texture(HeimSprite* sprite);
float heim_sprite_get_rotation(HeimSprite* sprite);

GLuint heim_sprite_get_vao(HeimSprite* sprite);
GLuint heim_sprite_get_vbo(HeimSprite* sprite);
GLuint heim_sprite_get_ebo(HeimSprite* sprite);

HeimMat4 heim_sprite_get_model(HeimSprite* sprite);
HeimMat4 heim_sprite_get_view(HeimSprite* sprite);
HeimMat4 heim_sprite_get_projection(HeimSprite* sprite);

void heim_sprite_free(HeimSprite* sprite);

#endif  // HEIM_SPRITE_H
