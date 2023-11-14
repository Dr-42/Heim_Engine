#pragma once

#include "math/heim_vec.h"

typedef struct {
    float m[4][4] __attribute__((aligned(16)));
} __attribute__((aligned(16))) HeimMat4;

typedef struct {
    float m[3][3] __attribute__((aligned(16)));
} __attribute__((aligned(16))) HeimMat3;

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

HeimMat3 heim_mat3_identity();
HeimMat3 heim_mat3_transpose(HeimMat3 mat);
HeimMat3 heim_mat3_inverse(HeimMat3 mat);
HeimMat3 heim_mat3_from_mat4(HeimMat4 mat);
