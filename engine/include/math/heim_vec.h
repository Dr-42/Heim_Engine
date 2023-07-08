#pragma once

#include <math.h>
#include <stdint.h>

/// @brief 2D Vector of uint64_t
typedef struct {
    uint64_t x, y;
} HeimVec2ui;

/// @brief 2D Vector of int64_t
typedef struct {
    int64_t x, y;
} HeimVec2i;

/// @brief 2D Vector of float
typedef struct {
    float x, y;
} HeimVec2f;

typedef struct {
    float x, y, z;
} HeimVec3f;

typedef struct {
    float x, y, z, w;
} HeimVec4f;

/// @brief Add two HeimVec2f vectors
/// @param a vec 1
/// @param b vec 2
/// @return
HeimVec2f heim_vec2f_add(HeimVec2f a, HeimVec2f b);
HeimVec2f heim_vec2f_sub(HeimVec2f a, HeimVec2f b);

HeimVec3f heim_vec3f_add(HeimVec3f a, HeimVec3f b);
HeimVec3f heim_vec3f_sub(HeimVec3f a, HeimVec3f b);
HeimVec3f heim_vec3f_mul(HeimVec3f a, float b);

float heim_vec3f_dot(HeimVec3f a, HeimVec3f b);
HeimVec3f heim_vec3f_cross(HeimVec3f a, HeimVec3f b);
HeimVec3f heim_vec3f_normalize(HeimVec3f a);

HeimVec3f heim_vec3f_from_vec4f(HeimVec4f a);