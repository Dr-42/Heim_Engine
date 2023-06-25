#include "math/heim_mat.h"

#include <stdio.h>

#include "math/heim_math_common.h"
#include "math/heim_vector.h"

void heim_mat3_print(HeimMat3 mat) {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            printf("[");
        }
        printf("%f ", mat.m[i]);

        if (i % 3 == 2) {
            printf("]\n");
        }
    }
}

void heim_mat4_print(HeimMat4 mat) {
    for (int i = 0; i < 16; i++) {
        if (i % 4 == 0) {
            printf("[");
        }
        printf("%f ", mat.m[i]);

        if (i % 4 == 3) {
            printf("]\n");
        }
    }
}

HeimMat3 heim_mat3_identity() {
    HeimMat3 mat = {0};
    mat.m[0] = 1.0f;
    mat.m[4] = 1.0f;
    mat.m[8] = 1.0f;
    return mat;
}

HeimMat4 heim_mat4_identity() {
    HeimMat4 mat = {0};
    mat.m[0] = 1.0f;
    mat.m[5] = 1.0f;
    mat.m[10] = 1.0f;
    mat.m[15] = 1.0f;
    return mat;
}

HeimMat3 heim_mat3_translate(HeimMat3 m, HeimVec2f vec) {
    HeimMat3 mat = m;
    mat.m[6] = vec.x;
    mat.m[7] = vec.y;
    return mat;
}

HeimMat4 heim_mat4_translate(HeimMat4 m, HeimVec3f vec) {
    HeimMat4 mat = m;
    mat.m[3] = vec.x;
    mat.m[7] = vec.y;
    mat.m[11] = vec.z;
    return mat;
}

HeimMat3 heim_mat3_rotate(HeimMat3 m, float degrees) {
    HeimMat3 mat = m;
    float radians = degrees * HEIM_PI / 180.0f;
    float c = heim_math_cos(radians);
    float s = heim_math_sin(radians);
    mat.m[0] = c;
    mat.m[1] = s;
    mat.m[3] = -s;
    mat.m[4] = c;
    return mat;
}
HeimMat4 heim_mat4_rotate(HeimMat4 m, float degrees, HeimVec3f axis) {
    HeimMat4 mat = m;
    float radians = degrees * HEIM_PI / 180.0f;
    float c = heim_math_cos(radians);
    float s = heim_math_sin(radians);
    float omc = 1.0f - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    mat.m[0] = x * x * omc + c;
    mat.m[1] = y * x * omc + z * s;
    mat.m[2] = x * z * omc - y * s;
    mat.m[4] = x * y * omc - z * s;
    mat.m[5] = y * y * omc + c;
    mat.m[6] = y * z * omc + x * s;
    mat.m[8] = x * z * omc + y * s;
    mat.m[9] = y * z * omc - x * s;
    mat.m[10] = z * z * omc + c;
    return mat;
}

HeimMat3 heim_mat3_scale(HeimMat3 m, HeimVec2f vec) {
    HeimMat3 mat = m;
    mat.m[0] = vec.x;
    mat.m[4] = vec.y;
    return mat;
}

HeimMat4 heim_mat4_scale(HeimMat4 m, HeimVec3f vec) {
    HeimMat4 mat = m;
    mat.m[0] = vec.x;
    mat.m[5] = vec.y;
    mat.m[10] = vec.z;
    return mat;
}

HeimMat3 heim_mat3_multiply(HeimMat3 a, HeimMat3 b) {
    HeimMat3 mat = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 3; ++k) {
                sum += a.m[i + k * 3] * b.m[k + j * 3];
            }
            mat.m[i + j * 3] = sum;
        }
    }
    return mat;
}
HeimMat4 heim_mat4_multiply(HeimMat4 a, HeimMat4 b) {
    HeimMat4 mat = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.m[i + k * 4] * b.m[k + j * 4];
            }
            mat.m[i + j * 4] = sum;
        }
    }
    return mat;
}

HeimVec2f heim_mat3_transform_vec2(HeimMat3 mat, HeimVec2f vec) {
    HeimVec2f result = {0};
    result.x = mat.m[0] * vec.x + mat.m[3] * vec.y + mat.m[6];
    result.y = mat.m[1] * vec.x + mat.m[4] * vec.y + mat.m[7];
    return result;
}

HeimVec3f heim_mat4_transform_vec3(HeimMat4 mat, HeimVec3f vec) {
    HeimVec3f result = {0};
    result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z + mat.m[12];
    result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z + mat.m[13];
    result.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z + mat.m[14];
    return result;
}

HeimMat4 heim_mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
    HeimMat4 mat = {0};
    mat.m[0] = 2.0f / (right - left);
    mat.m[5] = 2.0f / (top - bottom);
    mat.m[10] = -2.0f / (far - near);
    mat.m[12] = -(right + left) / (right - left);
    mat.m[13] = -(top + bottom) / (top - bottom);
    mat.m[14] = -(far + near) / (far - near);
    mat.m[15] = 1.0f;
    return mat;
}

HeimMat4 heim_mat4_perspective(float fov, float aspect, float near, float far) {
    HeimMat4 mat = {0};
    float tanHalfFov = heim_math_tan(fov * 0.5f);
    mat.m[0] = 1.0f / (aspect * tanHalfFov);
    mat.m[5] = 1.0f / tanHalfFov;
    mat.m[10] = -(far + near) / (far - near);
    mat.m[11] = -1.0f;
    mat.m[14] = -(2.0f * far * near) / (far - near);
    return mat;
}

HeimMat4 heim_mat4_lookat(HeimVec3f eye, HeimVec3f target, HeimVec3f up) {
    HeimVec3f z = heim_vec3f_normalize(heim_vec3f_sub(eye, target));
    HeimVec3f x = heim_vec3f_normalize(heim_vec3f_cross(up, z));
    HeimVec3f y = heim_vec3f_cross(z, x);
    HeimMat4 mat = {0};
    mat.m[0] = x.x;
    mat.m[1] = y.x;
    mat.m[2] = z.x;
    mat.m[4] = x.y;
    mat.m[5] = y.y;
    mat.m[6] = z.y;
    mat.m[8] = x.z;
    mat.m[9] = y.z;
    mat.m[10] = z.z;
    mat.m[12] = -heim_vec3f_dot(x, eye);
    mat.m[13] = -heim_vec3f_dot(y, eye);
    mat.m[14] = -heim_vec3f_dot(z, eye);
    mat.m[15] = 1.0f;
    return mat;
}
