#pragma once

#include <stdint.h>

#include "renderer/heim_obj.h"
#include "renderer/heim_texture.h"

typedef struct HeimModel {
    HeimObj* obj;
    HeimTexture* texture;
} HeimModel;
