#ifndef HEIM_MAT_H
#define HEIM_MAT_H

#include "math/heim_vector.h"

typedef struct {
    float m[4][4];
} HeimMat4;

void heim_mat4_print(HeimMat4 mat);

HeimMat4 heim_mat4_identity();

HeimMat4 heim_mat4_translate(HeimMat4 m, HeimVec3f vec);

HeimMat4 heim_mat4_rotate(HeimMat4 m, float degrees, HeimVec3f axis);

HeimMat4 heim_mat4_scale(HeimMat4 m, HeimVec3f vec);

HeimMat4 heim_mat4_multiply(HeimMat4 a, HeimMat4 b);

HeimVec3f heim_mat4_transform_vec3(HeimMat4 mat, HeimVec3f vec);

HeimMat4 heim_mat4_ortho(float left, float right, float bottom, float top, float near, float far);
HeimMat4 heim_mat4_perspective(float fov, float aspect, float near, float far);
HeimMat4 heim_mat4_lookat(HeimVec3f eye, HeimVec3f target, HeimVec3f up);

#endif  // HEIM_MAT_H
