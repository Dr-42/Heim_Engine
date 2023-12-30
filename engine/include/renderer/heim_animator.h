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

HeimAnimator* heim_animator_init(const char* path, HeimSkeletalModel* model);
void heim_animator_free(HeimAnimator* animator);
void heim_animator_update(HeimAnimator* animator, float dt);
void heim_animator_switch_animation(HeimAnimator* animator, size_t index);
void heim_animator_play_animation(HeimAnimator* animator, HeimAnimation* animation);
void heim_animator_calculate_bone_transform(HeimAnimator* animator, const struct assimp_node_data_t* node, HeimMat4 parent_transform);


