#pragma once

#include "ecs/heim_ecs.h"
#include "ecs/predef_comps/heim_model.h"
#include "ecs/predef_comps/heim_transform.h"

void heim_model_renderer_init();
void heim_model_renderer_system(HeimEntity entity, float dt);

void heim_model_renderer_free();
