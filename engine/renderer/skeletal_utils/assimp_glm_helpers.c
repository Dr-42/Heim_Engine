#include "renderer/skeletal_utils/assimp_glm_helpers.h"
#include "math/heim_vec.h"

HeimMat4 convert_matrix_to_glm(const struct aiMatrix4x4* matrix) {
    HeimMat4 dest;
    dest.m[0][0] = matrix->a1;
    dest.m[1][0] = matrix->a2;
    dest.m[2][0] = matrix->a3;
    dest.m[3][0] = matrix->a4;

    dest.m[0][1] = matrix->b1;
    dest.m[1][1] = matrix->b2;
    dest.m[2][1] = matrix->b3;
    dest.m[3][1] = matrix->b4;

    dest.m[0][2] = matrix->c1;
    dest.m[1][2] = matrix->c2;
    dest.m[2][2] = matrix->c3;
    dest.m[3][2] = matrix->c4;

    dest.m[0][3] = matrix->d1;
    dest.m[1][3] = matrix->d2;
    dest.m[2][3] = matrix->d3;
    dest.m[3][3] = matrix->d4;
    return dest;
}

HeimVec3f get_glm_vec(const struct aiVector3D* vec) {
    HeimVec3f dest;
    dest.x = vec->x;
    dest.y = vec->y;
    dest.z = vec->z;
    return dest;
}

HeimVec4f get_glm_quat(const struct aiQuaternion* quat) {
    HeimVec4f dest;
    dest.x = quat->x;
    dest.y = quat->y;
    dest.z = quat->z;
    dest.w = quat->w;
    return dest;
}
