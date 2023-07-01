#ifndef HEIM_UITRANSFORM_H
#define HEIM_UITRANSFORM_H

#include "math/heim_vec.h"

typedef struct HeimUiTransform {
    HeimVec2f position;
    HeimVec2f size;
    float rotation;
    HeimVec4f color;
} HeimUiTransform;

#endif  // HEIM_UITRANSFORM_H