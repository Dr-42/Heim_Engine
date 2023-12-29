#include "renderer/heim_animator.h"
#include "renderer/heim_animation.h"
#include "renderer/skeletal_utils/heim_bone.h"
#include "renderer/heim_skeletal_model.h"
#include "core/utils/heim_vector.h"
#include "math/heim_mat.h"
#include "math/heim_vec.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

HeimAnimator* animator_init(const char* path, HeimSkeletalModel* model) {
    HeimAnimator* animator = malloc(sizeof(HeimAnimator));
    memset(animator, 0, sizeof(HeimAnimator));
    animator->animations = heim_vector_create(HeimAnimation);
    const struct aiScene* scene = aiImportFile(path, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
        printf("%u] Animation name: %s\n",i, scene->mAnimations[i]->mName.data);
        HeimAnimation* animation = animation_init(scene, scene->mAnimations[i], model);
        heim_vector_push(animator->animations, animation);
    }
    animator->current_animation = animator->animations[0];
    animator->current_time = 0.0f;
    animator->blend_speed = 1.0f;
    animator->final_bone_matrices = heim_vector_reserve(HeimMat4, heim_vector_length(animator->current_animation->bones));
    heim_vector_length_set(animator->final_bone_matrices, heim_vector_length(animator->current_animation->bones));
    for (size_t i = 0; i < heim_vector_length(animator->current_animation->bones); i++) {
        animator->final_bone_matrices[i] = heim_mat4_identity();
    }
    aiReleaseImport(scene);
    return animator;
}

void animator_free(HeimAnimator* animator) {
    for (size_t i = 0; i < heim_vector_length(animator->animations); i++) {
        animation_free(animator->animations[i]);
    }
    heim_vector_destroy(animator->animations);
    heim_vector_destroy(animator->final_bone_matrices);
    free(animator);
}

void animator_update(HeimAnimator* animator, float dt) {
    animator->delta_time = dt;
    if (animator->current_animation) {
        animator->current_time += animator->current_animation->ticks_per_second * dt;
        animator->current_time = fmod(animator->current_time, animator->current_animation->duration);
        HeimMat4 identity = heim_mat4_identity();
        if(animator->blend_animation) {
            animator->blend_time += animator->blend_animation->ticks_per_second * dt;
            animator->blend_time = fmod(animator->blend_time, animator->blend_animation->duration);
            animator->blend_factor += dt * animator->blend_speed;
            if(animator->blend_factor >= 1.0f) {
                animator->current_animation = animator->blend_animation;
                animator->blend_animation = NULL;
                animator->blend_factor = 0.0f;
                animator->current_time = animator->blend_time;
                animator->blend_time = 0.0f;
            }
        }
        calculate_bone_transform(animator, &animator->current_animation->root_node, identity);
    }
}

void animator_play_animation(HeimAnimator* animator, HeimAnimation* animation) {
    animator->current_animation = animation;
    animator->current_time = 0.0f;
}

void calculate_bone_transform(HeimAnimator* animator, const struct assimp_node_data_t* node, HeimMat4 parent_transform) {
    char* node_name = node->name;
    HeimMat4 node_transform = node->transformation;
    HeimBone* bone = find_bone(animator->current_animation, node_name);
    HeimVec3f position;
    HeimVec4f rotation;
    HeimVec3f scale;
    if(bone) {
        bone_update(bone, animator->current_time);
        position = bone->local_position;
        rotation = bone->local_rotation;
        scale = bone->local_scale;
        if(animator->blend_animation) {
            HeimBone* blend_bone = find_bone(animator->blend_animation, node_name);
            if(blend_bone) {
                bone_update(blend_bone, animator->blend_time);
                HeimVec3f blend_position = blend_bone->local_position;
                HeimVec4f blend_rotation = blend_bone->local_rotation;
                HeimVec3f blend_scale = blend_bone->local_scale;
                position = heim_vec3f_mix(position, blend_position, animator->blend_factor);
                rotation = heim_quat_slerp(rotation, blend_rotation, animator->blend_factor);
                scale = heim_vec3f_mix(scale, blend_scale, animator->blend_factor);
                HeimMat4 identity = heim_mat4_identity();
                HeimMat4 translation = heim_mat4_translate(identity, position);
                HeimMat4 rotation_mat = heim_mat4_from_quat(rotation);
                HeimMat4 scale_mat = heim_mat4_scale(identity, scale);
                node_transform = heim_mat4_multiply(translation, heim_mat4_multiply(rotation_mat, scale_mat));
            }
        } else {
            HeimMat4 identity = heim_mat4_identity();
            HeimMat4 translation = heim_mat4_translate(identity, position);
            HeimMat4 rotation_mat = heim_mat4_from_quat(rotation);
            HeimMat4 scale_mat = heim_mat4_scale(identity, scale);
            node_transform = heim_mat4_multiply(translation, heim_mat4_multiply(rotation_mat, scale_mat));
        }
    }
    HeimMat4 global_transform = heim_mat4_multiply(parent_transform, node_transform);
    heim_bone_info_t* bone_info_map = animator->current_animation->bone_info_map;
    if(find_bone_index(bone_info_map, node_name) != -1) {
        int32_t bone_idx = find_bone_index(bone_info_map, node_name);
        heim_bone_info_t bone_info = bone_info_map[bone_idx];
        size_t bone_index = bone_info.id;
        HeimMat4 offset = bone_info.offset;
        animator->final_bone_matrices[bone_index] = heim_mat4_multiply(global_transform, offset);
    }
    for(int32_t i = 0; i < node->children_count; i++) {
        calculate_bone_transform(animator, &node->children[i], global_transform);
    }
}

void animator_switch_animation(HeimAnimator* animator, size_t index){
    if(index < heim_vector_length(animator->animations)) {
        /*
        animator->current_animation = animator->animations[index];
        animator->current_time = 0.0f;
        */
        animator->blend_animation = animator->animations[index];
    }
}
