#pragma once

#include "ecs/predef_comps/heim_camera.h"
#include "ecs/predef_comps/heim_transform.h"
#include "renderer/heim_skybox.h"

typedef struct HeimWorld {
    HeimSkybox* skybox;
    HeimCamera* camera;
    HeimTransform* camera_transform;
} HeimWorld;
