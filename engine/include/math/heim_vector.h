#ifndef HEIM_VECTOR_H
#define HEIM_VECTOR_H

#include <stdint.h>

/// @brief 2D Vector of uint64_t
typedef struct{
	uint64_t x, y;
} HeimVec2ui;

/// @brief 2D Vector of int64_t
typedef struct{
	int64_t x, y;
} HeimVec2i;

/// @brief 2D Vector of float
typedef struct{
	float x, y;
} HeimVec2f;

/// @brief Add two HeimVec2f vectors
/// @param a vec 1
/// @param b vec 2
/// @return 
HeimVec2f heim_vec2f_add(HeimVec2f a, HeimVec2f b);

#endif // HEIM_VECTOR_H 
