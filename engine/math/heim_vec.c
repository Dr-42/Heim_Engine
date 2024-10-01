#include "math/heim_vec.h"

float heim_deg_to_rad(float deg) { return deg * (M_PI / 180.0f); }

HeimVec2f heim_vec2f_add(HeimVec2f a, HeimVec2f b) { return (HeimVec2f){a.x + b.x, a.y + b.y}; }

HeimVec2f heim_vec2f_sub(HeimVec2f a, HeimVec2f b) { return (HeimVec2f){a.x - b.x, a.y - b.y}; }

HeimVec3f heim_vec3f_add(HeimVec3f a, HeimVec3f b) { return (HeimVec3f){a.x + b.x, a.y + b.y, a.z + b.z}; }

HeimVec3f heim_vec3f_sub(HeimVec3f a, HeimVec3f b) { return (HeimVec3f){a.x - b.x, a.y - b.y, a.z - b.z}; }

HeimVec3f heim_vec3f_mul(HeimVec3f a, float b) { return (HeimVec3f){a.x * b, a.y * b, a.z * b}; }

float heim_vec3f_dot(HeimVec3f a, HeimVec3f b) { return (a.x * b.x + a.y * b.y + a.z * b.z); }

HeimVec3f heim_vec3f_cross(HeimVec3f a, HeimVec3f b) {
    return (HeimVec3f){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

HeimVec3f heim_vec3f_normalize(HeimVec3f a) {
    float length = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    return (HeimVec3f){a.x / length, a.y / length, a.z / length};
}

HeimVec3f heim_vec3f_from_vec4f(HeimVec4f a) { return (HeimVec3f){a.x, a.y, a.z}; }

HeimVec3f heim_vec3f_mix(HeimVec3f a, HeimVec3f b, float t) {
    return (HeimVec3f){a.x * (1.0f - t) + b.x * t, a.y * (1.0f - t) + b.y * t, a.z * (1.0f - t) + b.z * t};
}

HeimVec4f heim_vec4f_normalize(HeimVec4f a) {
    float length = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
    return (HeimVec4f){a.x / length, a.y / length, a.z / length, a.w / length};
}

HeimVec4f heim_vec4f_mix(HeimVec4f a, HeimVec4f b, float t) {
    return (HeimVec4f){a.x * (1.0f - t) + b.x * t, a.y * (1.0f - t) + b.y * t, a.z * (1.0f - t) + b.z * t,
                       a.w * (1.0f - t) + b.w * t};
}

float heim_vec4f_dot(HeimVec4f a, HeimVec4f b) { return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w); }

HeimVec4f heim_vec4f_from_vec3f(HeimVec3f a) { return (HeimVec4f){a.x, a.y, a.z, 0.0f}; }

HeimVec4f heim_vec4f_mul(HeimVec4f a, float b) { return (HeimVec4f){a.x * b, a.y * b, a.z * b, a.w * b}; }

HeimVec4f heim_quat_slerp(HeimVec4f a, HeimVec4f b, float t) {
    HeimVec4f result = {0};
    float dot = heim_vec4f_dot(a, b);
    if (dot < 0.0f) {
        b = heim_vec4f_mul(b, -1.0f);
        dot = -dot;
    }
    if (dot > 0.9995f) {
        result = heim_vec4f_mix(a, b, t);
        return result;
    }
    float theta_0 = acosf(dot);
    float theta = theta_0 * t;
    float sin_theta = sinf(theta);
    float sin_theta_0 = sinf(theta_0);
    float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;
    result.x = s0 * a.x + s1 * b.x;
    result.y = s0 * a.y + s1 * b.y;
    result.z = s0 * a.z + s1 * b.z;
    result.w = s0 * a.w + s1 * b.w;
    return result;
}
