#ifndef HEIM_MODEL_RENDERER_H
#define HEIM_MODEL_RENDERER_H

#include "ecs/heim_ecs.h"
#include "ecs/predef_comps/heim_model.h"
#include "ecs/predef_comps/heim_transform.h"

void heim_model_render(HeimModel* model, HeimVec3f position, HeimVec3f rotation, HeimVec3f scale, float* dt);

void heim_model_renderer_system(HeimEcs* ecs, HeimEntity entity, float dt);

#endif  // HEIM_MODEL_RENDERER_H