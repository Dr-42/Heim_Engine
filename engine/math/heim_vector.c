#include "math/heim_vector.h"

HeimVec2f heim_vec2f_add(HeimVec2f a, HeimVec2f b) {
    return (HeimVec2f){a.x + b.x, a.y + b.y};
}

HeimVec3f heim_vec3f_add(HeimVec3f a, HeimVec3f b) {
    return (HeimVec3f){a.x + b.x, a.y + b.y, a.z + b.z};
}

HeimVec3f heim_vec3f_sub(HeimVec3f a, HeimVec3f b) {
    return (HeimVec3f){a.x - b.x, a.y - b.y, a.z - b.z};
}

HeimVec3f heim_vec3f_mul(HeimVec3f a, float b) {
    return (HeimVec3f){a.x * b, a.y * b, a.z * b};
}

float heim_vec3f_dot(HeimVec3f a, HeimVec3f b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

HeimVec3f heim_vec3f_cross(HeimVec3f a, HeimVec3f b) {
    return (HeimVec3f){a.y * b.z - a.z * b.y,
                       a.z * b.x - a.x * b.z,
                       a.x * b.y - a.y * b.x};
}

HeimVec3f heim_vec3f_normalize(HeimVec3f a) {
    float length = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    return (HeimVec3f){a.x / length, a.y / length, a.z / length};
}
