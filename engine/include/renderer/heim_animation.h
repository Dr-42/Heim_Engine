#pragma once

#include <stdint.h>

#include "renderer/heim_skeletal_model.h"
#include "renderer/skeletal_utils/heim_bone.h"
#include "math/heim_mat.h"

typedef struct assimp_node_data_t assimp_node_data_t;

typedef struct assimp_node_data_t {
    HeimMat4 transformation;
    char* name;
    int32_t children_count;
    assimp_node_data_t* children;
} assimp_node_data_t;

typedef struct HeimAnimation {
    float duration;
    int32_t ticks_per_second;
    HeimBone** bones;
    assimp_node_data_t root_node;
    heim_bone_info_t* bone_info_map;
    char* name;
} HeimAnimation;

HeimAnimation* animation_init(const struct aiScene* scene, struct aiAnimation* animation, HeimSkeletalModel* model);
void animation_free(HeimAnimation* animation);
HeimBone* find_bone(HeimAnimation* animation, const char* name);
