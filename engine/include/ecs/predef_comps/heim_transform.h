#ifndef HEIM_TRANSFORM_H
#define HEIM_TRANSFORM_H

#include "math/heim_vec.h"

typedef struct HeimTransform {
    HeimVec3f position;
    HeimVec3f size;
    HeimVec3f rotation;
    HeimVec4f color;
} HeimTransform;

#endif  // HEIM_TRANSFORM_H