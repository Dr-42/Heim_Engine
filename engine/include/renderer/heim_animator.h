#pragma once

#include "renderer/heim_animation.h"

typedef struct HeimAnimator {
    HeimMat4* final_bone_matrices;
    HeimAnimation* current_animation;
    HeimAnimation* blend_animation;
    float blend_factor;
    float blend_speed;
    HeimAnimation** animations;
    float current_time;
    float blend_time;
    float delta_time;
} HeimAnimator;

HeimAnimator* animator_init(const char* path, HeimSkeletalModel* model);
void animator_free(HeimAnimator* animator);
void animator_update(HeimAnimator* animator, float dt);
void animator_switch_animation(HeimAnimator* animator, size_t index);
void animator_play_animation(HeimAnimator* animator, HeimAnimation* animation);
void calculate_bone_transform(HeimAnimator* animator, const struct assimp_node_data_t* node, HeimMat4 parent_transform);


