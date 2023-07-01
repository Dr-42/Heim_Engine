#ifndef HEIM_MODEL_H
#define HEIM_MODEL_H

#include <stdint.h>

#include "renderer/heim_obj.h"
#include "renderer/heim_texture.h"

typedef struct HeimModel {
    HeimObj* obj;
    HeimTexture* texture;
} HeimModel;

#endif  // HEIM_MODEL_H