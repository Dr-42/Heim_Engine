#ifndef HEIM_MATH_COMMON_H
#define HEIM_MATH_COMMON_H

#include <stdint.h>

#define HEIM_PI 3.14159265358979323846f

float heim_math_deg_to_rad(float deg);
float heim_math_rad_to_deg(float rad);

float heim_math_clamp(float value, float min, float max);
float heim_math_lerp(float a, float b, float t);

float heim_math_min(float a, float b);
float heim_math_max(float a, float b);

float heim_math_abs(float a);

float heim_math_sin(float a);
float heim_math_cos(float a);
float heim_math_tan(float a);

float heim_math_asin(float a);
float heim_math_acos(float a);
float heim_math_atan(float a);

#endif  // HEIM_MATH_COMMON_H