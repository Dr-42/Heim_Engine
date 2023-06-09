#pragma once

#include "ecs/heim_ecs.h"
#include "ecs/predef_comps/heim_pbr_model.h"
#include "ecs/predef_comps/heim_transform.h"

void heim_pbr_model_renderer_init(const char* skybox_path);

void heim_pbr_model_renderer_system(HeimEntity entity, float dt);

void heim_pbr_model_renderer_free();
