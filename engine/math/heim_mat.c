#include "math/heim_mat.h"

#include <stdio.h>

#include "math/heim_math_common.h"
#include "math/heim_vec.h"

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
    HeimMat4 res = heim_mat4_identity();
    res.m[3][0] = m.m[0][0] * vec.x + m.m[1][0] * vec.y + m.m[2][0] * vec.z + m.m[3][0];
    res.m[3][1] = m.m[0][1] * vec.x + m.m[1][1] * vec.y + m.m[2][1] * vec.z + m.m[3][1];
    res.m[3][2] = m.m[0][2] * vec.x + m.m[1][2] * vec.y + m.m[2][2] * vec.z + m.m[3][2];
    res.m[3][3] = m.m[0][3] * vec.x + m.m[1][3] * vec.y + m.m[2][3] * vec.z + m.m[3][3];
    return res;
}

HeimMat4 heim_mat4_rotate(HeimMat4 m, float degrees, HeimVec3f axis) {
    HeimMat4 rot = {0};
    float radians = degrees * HEIM_PI / 180.0f;
    float c = cosf(radians);
    float s = sinf(radians);    
    float omc = 1.0f - c;
    
    HeimVec3f norm = heim_vec3f_normalize(axis);
    float x = norm.x;
    float y = norm.y;
    float z = norm.z;

    rot.m[0][0] = x * x * omc + c;
    rot.m[0][1] = y * x * omc + z * s;
    rot.m[0][2] = x * z * omc - y * s;
    rot.m[0][3] = 0.0f;

    rot.m[1][0] = x * y * omc - z * s;
    rot.m[1][1] = y * y * omc + c;
    rot.m[1][2] = y * z * omc + x * s;
    rot.m[1][3] = 0.0f;

    rot.m[2][0] = x * z * omc + y * s;
    rot.m[2][1] = y * z * omc - x * s;
    rot.m[2][2] = z * z * omc + c;
    rot.m[2][3] = 0.0f;

    rot.m[3][0] = 0.0f;
    rot.m[3][1] = 0.0f;
    rot.m[3][2] = 0.0f;
    rot.m[3][3] = 1.0f;

    HeimMat4 res = heim_mat4_multiply(m, rot);
    return res;
}

HeimMat4 heim_mat4_scale(HeimMat4 m, HeimVec3f vec) {
    HeimMat4 scale = heim_mat4_identity();
    scale.m[0][0] = vec.x;
    scale.m[1][1] = vec.y;
    scale.m[2][2] = vec.z;
    HeimMat4 res = heim_mat4_multiply(m, scale);
    return res;
}

HeimMat4 heim_mat4_multiply(HeimMat4 a, HeimMat4 b) {
    HeimMat4 mat = {0};
    for(int i = 0; i < 4; i++) {    
        for(int j = 0; j < 4; j++) {    
            mat.m[i][j] = 0;    
            for(int k = 0; k < 4; k++){    
                mat.m[i][j] += b.m[i][k] * a.m[k][j];    
            }    
        }    
    }  
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
    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = -1.0f / (far - near);

    mat.m[0][0] = 2.0f * rl;
    mat.m[1][1] = 2.0f * tb;
    mat.m[2][2] = 2.0f * fn;
    mat.m[3][0] = -(right + left) * rl;
    mat.m[3][1] = -(top + bottom) * tb;
    mat.m[3][2] = (far + near) * fn;
    mat.m[3][3] = 1.0f;
    return mat;
}

HeimMat4 heim_mat4_perspective(float fov, float aspect, float near, float far) {
    HeimMat4 mat = {0};
    float f = 1.0f / tanf(fov * 0.5f * HEIM_PI / 180.0f);
    float fn = 1.0f / (near - far);

    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = (near + far) * fn;
    mat.m[2][3] = -1.0f;
    mat.m[3][2] = 2.0f * near * far * fn;

    return mat;
}

HeimMat4 heim_mat4_lookat(HeimVec3f eye, HeimVec3f target, HeimVec3f up) {
    HeimMat4 mat = {0};
    HeimVec3f f = heim_vec3f_normalize(heim_vec3f_sub(target, eye));
    HeimVec3f s = heim_vec3f_normalize(heim_vec3f_cross(f, up));
    HeimVec3f u = heim_vec3f_cross(s, f);

    mat.m[0][0] = s.x;
    mat.m[1][0] = s.y;
    mat.m[2][0] = s.z;
    mat.m[0][1] = u.x;
    mat.m[1][1] = u.y;
    mat.m[2][1] = u.z;
    mat.m[0][2] = -f.x;
    mat.m[1][2] = -f.y;
    mat.m[2][2] = -f.z;
    mat.m[3][0] = -heim_vec3f_dot(s, eye);
    mat.m[3][1] = -heim_vec3f_dot(u, eye);
    mat.m[3][2] = heim_vec3f_dot(f, eye);
    mat.m[3][3] = 1.0f;

    return mat;
}

HeimMat3 heim_mat3_identity() {
    HeimMat3 mat = {0};
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    return mat;
}
HeimMat3 heim_mat3_transpose(HeimMat3 mat) {
    HeimMat3 result = mat;
    mat.m[0][1] = result.m[1][0];
    mat.m[0][2] = result.m[2][0];
    mat.m[1][0] = result.m[0][1];
    mat.m[1][2] = result.m[2][1];
    mat.m[2][0] = result.m[0][2];
    mat.m[2][1] = result.m[1][2];
    return result;
}

float heim_mat3_determinant(HeimMat3 mat) {
    float det = 0.0f;
    for (size_t i = 0; i < 3; i++) {
        det += (mat.m[0][i] * (mat.m[1][(i + 1) % 3] * mat.m[2][(i + 2) % 3] - mat.m[1][(i + 2) % 3] * mat.m[2][(i + 1) % 3]));
    }
    return det;
}

HeimMat3 heim_mat3_inverse(HeimMat3 mat) {
    HeimMat3 result = {0};
    float det = heim_mat3_determinant(mat);
    if (det == 0.0f) {
        return result;
    }
    float invdet = 1.0f / det;
    
    result.m[0][0] = (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) * invdet;
    result.m[0][1] = (mat.m[0][2] * mat.m[2][1] - mat.m[0][1] * mat.m[2][2]) * invdet;
    result.m[0][2] = (mat.m[0][1] * mat.m[1][2] - mat.m[0][2] * mat.m[1][1]) * invdet;
    result.m[1][0] = (mat.m[1][2] * mat.m[2][0] - mat.m[1][0] * mat.m[2][2]) * invdet;
    result.m[1][1] = (mat.m[0][0] * mat.m[2][2] - mat.m[0][2] * mat.m[2][0]) * invdet;
    result.m[1][2] = (mat.m[0][2] * mat.m[1][0] - mat.m[0][0] * mat.m[1][2]) * invdet;
    result.m[2][0] = (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]) * invdet;
    result.m[2][1] = (mat.m[0][1] * mat.m[2][0] - mat.m[0][0] * mat.m[2][1]) * invdet;
    result.m[2][2] = (mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0]) * invdet;


    return result;
}

HeimMat3 heim_mat3_from_mat4(HeimMat4 mat) {
    HeimMat3 result = {0};
    result.m[0][0] = mat.m[0][0];
    result.m[0][1] = mat.m[0][1];
    result.m[0][2] = mat.m[0][2];
    result.m[1][0] = mat.m[1][0];
    result.m[1][1] = mat.m[1][1];
    result.m[1][2] = mat.m[1][2];
    result.m[2][0] = mat.m[2][0];
    result.m[2][1] = mat.m[2][1];
    result.m[2][2] = mat.m[2][2];
    return result;
}
