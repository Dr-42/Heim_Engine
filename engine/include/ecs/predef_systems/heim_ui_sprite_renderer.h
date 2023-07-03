#ifndef HEIM_UI_SPRITE_RENDER_H
#define HEIM_UI_SPRITE_RENDER_H

#include "ecs/heim_ecs.h"

void heim_ui_sprite_render_system_init();

void heim_ui_sprite_render_system(HeimEntity entity, float dt);

void heim_ui_sprite_render_system_free();

#endif  // HEIM_UI_SPRITE_RENDER_H
