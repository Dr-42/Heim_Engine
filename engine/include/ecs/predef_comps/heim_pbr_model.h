#pragma once

#include <stdint.h>

#include "renderer/heim_obj.h"
#include "renderer/heim_texture.h"

typedef struct HeimPBRModel {
    HeimObj* obj;
    HeimTexture* albedoMap;
    HeimTexture* normalMap;
    HeimTexture* metallicMap;
    HeimTexture* roughnessMap;
    HeimTexture* aoMap;
} HeimPBRModel;
