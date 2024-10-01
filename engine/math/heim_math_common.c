#include "math/heim_math_common.h"

#include <math.h>

float heim_math_deg_to_rad(float deg) { return deg * HEIM_PI / 180.0f; }
float heim_math_rad_to_deg(float rad) { return rad * 180.0f / HEIM_PI; }

float heim_math_clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
float heim_math_lerp(float a, float b, float t) { return a + (b - a) * t; }

float heim_math_min(float a, float b) { return a < b ? a : b; }
float heim_math_max(float a, float b) { return a > b ? a : b; }

float heim_math_abs(float a) { return a < 0.0f ? -a : a; }

float heim_math_sin(float a) { return sinf(a); }
float heim_math_cos(float a) { return cosf(a); }
float heim_math_tan(float a) { return tanf(a); }

float heim_math_asin(float a) { return asinf(a); }
float heim_math_acos(float a) { return acosf(a); }
float heim_math_atan(float a) { return atanf(a); }
