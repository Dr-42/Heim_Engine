#ifndef HEIM_CAMERA_H
#define HEIM_CAMERA_H

#include "math/heim_vec.h"

typedef struct HeimCamera {
    float fov;
    float aspect;
    float near;
    float far;

    float yaw;
    float pitch;

    HeimVec3f front;
    HeimVec3f up;
} HeimCamera;

#endif  // HEIM_CAMERA_H
