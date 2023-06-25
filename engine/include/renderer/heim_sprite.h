#ifndef HEIM_SPRITE_H
#define HEIM_SPRITE_H

#include <GL/glew.h>

#include "math/heim_vector.h"

typedef struct HeimSprite HeimSprite;

HeimSprite* heim_create_sprite(char* texture_path);
void heim_sprite_set_position(HeimSprite* sprite, HeimVec2f position);
void heim_sprite_set_size(HeimSprite* sprite, HeimVec2f size);
void heim_sprite_set_color(HeimSprite* sprite, HeimVec4f color);

HeimVec2f heim_sprite_get_position(HeimSprite* sprite);
HeimVec2f heim_sprite_get_size(HeimSprite* sprite);
HeimVec4f heim_sprite_get_color(HeimSprite* sprite);
GLuint heim_sprite_get_texture(HeimSprite* sprite);

GLuint heim_sprite_get_vao(HeimSprite* sprite);
GLuint heim_sprite_get_vbo(HeimSprite* sprite);
GLuint heim_sprite_get_ebo(HeimSprite* sprite);

void heim_sprite_free(HeimSprite* sprite);

#endif  // HEIM_SPRITE_H
