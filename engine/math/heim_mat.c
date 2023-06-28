#include "math/heim_mat.h"

#include <cglm/cglm.h>
#include <stdio.h>

#include "math/heim_math_common.h"

void heim_mat4_print(HeimMat4 mat) {
    for (size_t i = 0; i < 4; i++) {
        printf("[");
        for (size_t j = 0; j < 4; j++) {
            printf("%f ", mat.m[i][j]);
        }
        printf("]\n");
    }
}

HeimMat4 heim_mat4_identity() {
    HeimMat4 mat = {0};
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
    return mat;
}

HeimMat4 heim_mat4_translate(HeimMat4 m, HeimVec3f vec) {
    HeimMat4 mat = m;
    glm_translate(mat.m, (vec3){vec.x, vec.y, vec.z});
    return mat;
}

HeimMat4 heim_mat4_rotate(HeimMat4 m, float degrees, HeimVec3f axis) {
    HeimMat4 mat = m;
    float radians = degrees * HEIM_PI / 180.0f;
    glm_rotate(mat.m, radians, (vec3){axis.x, axis.y, axis.z});
    return mat;
}

HeimMat4 heim_mat4_scale(HeimMat4 m, HeimVec3f vec) {
    HeimMat4 mat = m;
    glm_scale(mat.m, (vec3){vec.x, vec.y, vec.z});
    return mat;
}

HeimMat4 heim_mat4_multiply(HeimMat4 a, HeimMat4 b) {
    HeimMat4 mat = {0};
    glm_mul(a.m, b.m, mat.m);
    return mat;
}

HeimVec3f heim_mat4_transform_vec3(HeimMat4 mat, HeimVec3f vec) {
    HeimVec3f result = {0};
    result.x = mat.m[0][0] * vec.x + mat.m[1][0] * vec.y + mat.m[2][0] * vec.z + mat.m[3][0];
    result.y = mat.m[0][1] * vec.x + mat.m[1][1] * vec.y + mat.m[2][1] * vec.z + mat.m[3][1];
    result.z = mat.m[0][2] * vec.x + mat.m[1][2] * vec.y + mat.m[2][2] * vec.z + mat.m[3][2];
    return result;
}

HeimMat4 heim_mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
    HeimMat4 mat = {0};
    glm_ortho(left, right, bottom, top, near, far, mat.m);
    return mat;
}

HeimMat4 heim_mat4_perspective(float fov, float aspect, float near, float far) {
    HeimMat4 mat = {0};
    glm_perspective(fov, aspect, near, far, mat.m);
    return mat;
}

HeimMat4 heim_mat4_lookat(HeimVec3f eye, HeimVec3f target, HeimVec3f up) {
    HeimMat4 mat = {0};
    glm_lookat((vec3){eye.x, eye.y, eye.z}, (vec3){target.x, target.y, target.z}, (vec3){up.x, up.y, up.z}, mat.m);
    return mat;
}
