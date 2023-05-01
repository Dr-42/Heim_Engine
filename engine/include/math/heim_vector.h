#ifndef HEIM_VECTOR_H
#define HEIM_VECTOR_H

#include <stdint.h>

typedef struct{
	uint32_t x, y;
} HeimVec2ui;

typedef struct{
	int32_t x, y;
} HeimVec2i;

typedef struct{
	float x, y;
} HeimVec2f;

HeimVec2f heim_vec2f_add(HeimVec2f a, HeimVec2f b);

#endif // HEIM_VECTOR_H 
