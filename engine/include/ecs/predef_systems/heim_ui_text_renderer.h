#pragma once

#include "ecs/heim_ecs.h"

void heim_ui_text_render_system_init();
void heim_ui_text_render_system(HeimEntity entity, float dt);
void heim_ui_text_render_system_free();