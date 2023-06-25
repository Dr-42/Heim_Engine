#ifndef HEIM_MAT_H
#define HEIM_MAT_H

#include "math/heim_vector.h"

typedef struct {
    float m[9];
} HeimMat3;

typedef struct {
    float m[16];
} HeimMat4;

void heim_mat3_print(HeimMat3 mat);
void heim_mat4_print(HeimMat4 mat);

HeimMat3 heim_mat3_identity();
HeimMat4 heim_mat4_identity();

HeimMat3 heim_mat3_translate(HeimMat3 m, HeimVec2f vec);
HeimMat4 heim_mat4_translate(HeimMat4 m, HeimVec3f vec);

HeimMat3 heim_mat3_rotate(HeimMat3 m, float degrees);
HeimMat4 heim_mat4_rotate(HeimMat4 m, float degrees, HeimVec3f axis);

HeimMat3 heim_mat3_scale(HeimMat3 m, HeimVec2f vec);
HeimMat4 heim_mat4_scale(HeimMat4 m, HeimVec3f vec);

HeimMat3 heim_mat3_multiply(HeimMat3 a, HeimMat3 b);
HeimMat4 heim_mat4_multiply(HeimMat4 a, HeimMat4 b);

HeimVec2f heim_mat3_transform_vec2(HeimMat3 mat, HeimVec2f vec);
HeimVec3f heim_mat4_transform_vec3(HeimMat4 mat, HeimVec3f vec);

HeimMat4 heim_mat4_ortho(float left, float right, float bottom, float top, float near, float far);
HeimMat4 heim_mat4_perspective(float fov, float aspect, float near, float far);
HeimMat4 heim_mat4_lookat(HeimVec3f eye, HeimVec3f target, HeimVec3f up);

#endif  // HEIM_MAT_H
