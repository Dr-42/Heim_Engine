#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <stdint.h>

#include "math/heim_vec.h"

typedef struct key_position_t {
	HeimVec3f position;
	float time_stamp;
} key_position_t;

typedef struct key_rotation_t {
	HeimVec4f orientation;
	float time_stamp;
} key_rotation_t;

typedef struct key_scale_t {
	HeimVec3f scale;
	float time_stamp;
} key_scale_t;

typedef struct HeimBone {
	key_position_t* positions;
	key_rotation_t* rotations;
	key_scale_t* scales;
	int32_t num_positions;
	int32_t num_rotations;
	int32_t num_scales;

	HeimVec3f local_position;
	HeimVec4f local_rotation;
	HeimVec3f local_scale;

	char* name;
	int32_t id;
} HeimBone;

HeimBone* bone_init(const char* name, int32_t id, const struct aiNodeAnim* channel);
void bone_free(HeimBone* bone);
void bone_update(HeimBone* bone, float animation_time);
int32_t get_poisiton_index(HeimBone* bone, float animation_time);
int32_t get_rotation_index(HeimBone* bone, float animation_time);
int32_t get_scale_index(HeimBone* bone, float animation_time);

