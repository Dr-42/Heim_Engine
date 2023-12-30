#include "renderer/skeletal_utils/heim_bone.h"

#include <assert.h>
#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"
#include "renderer/skeletal_utils/assimp_glm_helpers.h"
#include "math/heim_vec.h"

float get_scale_factor(float last_time_stamp, float next_time_stamp, float animation_time);
HeimVec3f interpolate_position(HeimBone* bone, float animation_time);
HeimVec4f interpolate_rotation(HeimBone* bone, float animation_time);
HeimVec3f interpolate_scaling(HeimBone* bone, float animation_time);
int32_t heim_bone_get_poisiton_index(HeimBone* bone, float animation_time);
int32_t heim_bone_get_rotation_index(HeimBone* bone, float animation_time);
int32_t heim_bone_get_scale_index(HeimBone* bone, float animation_time);

HeimBone* heim_bone_init(const char* name, int32_t id, const struct aiNodeAnim* channel) {
    HeimBone* bone = HEIM_MALLOC(HeimBone, HEIM_MEMORY_TYPE_RENDERER);
    memset(bone, 0, sizeof(HeimBone));
    bone->name = heim_malloc(strlen(name) + 1, HEIM_MEMORY_TYPE_RENDERER, __FILE__, __LINE__);
    strcpy(bone->name, name);
    bone->id = id;
    bone->num_positions = channel->mNumPositionKeys;
    bone->positions = heim_vector_create(key_position_t);
    bone->rotations = heim_vector_create(key_rotation_t);
    bone->scales = heim_vector_create(key_scale_t);

    for (int32_t position_idx = 0; position_idx < bone->num_positions; position_idx++) {
        struct aiVector3D ai_position = channel->mPositionKeys[position_idx].mValue;
        float time_stamp = channel->mPositionKeys[position_idx].mTime;
        key_position_t data;
        data.position = get_glm_vec(&ai_position);
        data.time_stamp = time_stamp;
        heim_vector_push(bone->positions, data);
    }
    bone->num_rotations = channel->mNumRotationKeys;
    for (int32_t rotation_idx = 0; rotation_idx < bone->num_rotations; rotation_idx++) {
        struct aiQuaternion ai_rotation = channel->mRotationKeys[rotation_idx].mValue;
        float time_stamp = channel->mRotationKeys[rotation_idx].mTime;
        key_rotation_t data;
        data.orientation = get_glm_quat(&ai_rotation);
        data.time_stamp = time_stamp;
        heim_vector_push(bone->rotations, data);
    }

    bone->num_scales = channel->mNumScalingKeys;
    for (int32_t scale_idx = 0; scale_idx < bone->num_scales; scale_idx++) {
        struct aiVector3D ai_scale = channel->mScalingKeys[scale_idx].mValue;
        float time_stamp = channel->mScalingKeys[scale_idx].mTime;
        key_scale_t data;
        data.scale = get_glm_vec(&ai_scale);
        data.time_stamp = time_stamp;
        heim_vector_push(bone->scales, data);
    }
    return bone;
}

void heim_bone_free(HeimBone* bone){
    HEIM_FREE(bone->name, HEIM_MEMORY_TYPE_RENDERER);
    heim_vector_destroy(bone->positions);
    heim_vector_destroy(bone->rotations);
    heim_vector_destroy(bone->scales);
    HEIM_FREE(bone, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_bone_update(HeimBone* bone, float animation_time) {
    bone->local_position = interpolate_position(bone, animation_time);
    bone->local_rotation = interpolate_rotation(bone, animation_time);
    bone->local_scale = interpolate_scaling(bone, animation_time);
}

int32_t heim_bone_get_poisiton_index(HeimBone* bone, float animation_time) {
    if (bone->num_positions == 1) {
        return 0;
    }
    for (int32_t i = 0; i < bone->num_positions - 1; i++) {
        if (animation_time < bone->positions[i + 1].time_stamp) {
            return i;
        }
    }
    assert(0);
    return 0;
}

int32_t heim_bone_get_rotation_index(HeimBone* bone, float animation_time) {
    if (bone->num_rotations == 1) {
        return 0;
    }
    for (int32_t i = 0; i < bone->num_rotations - 1; i++) {
        if (animation_time < bone->rotations[i + 1].time_stamp) {
            return i;
        }
    }
    assert(0);
    return 0;
}

int32_t heim_bone_get_scale_index(HeimBone* bone, float animation_time) {
    if (bone->num_scales == 1) {
        return 0;
    }
    for (int32_t i = 0; i < bone->num_scales - 1; i++) {
        if (animation_time < bone->scales[i + 1].time_stamp) {
            return i;
        }
    }
    assert(0);
    return 0;
}

float get_scale_factor(float last_time_stamp, float next_time_stamp, float animation_time) {
    float mid_way_length = animation_time - last_time_stamp;
    float frames_length = next_time_stamp - last_time_stamp;
    return mid_way_length / frames_length;
}

HeimVec3f interpolate_position(HeimBone* bone, float animation_time) {
    if (bone->num_positions == 1) {
        return bone->positions[0].position;
    }
    int32_t p0_index = heim_bone_get_poisiton_index(bone, animation_time);
    int32_t p1_index = p0_index + 1;
    float scale_factor = get_scale_factor(bone->positions[p0_index].time_stamp, bone->positions[p1_index].time_stamp, animation_time);
    HeimVec3f final_position = heim_vec3f_mix(bone->positions[p0_index].position, bone->positions[p1_index].position, scale_factor);
    //dest = heim_mat4_translate(dest, final_position);
    return final_position;
}

HeimVec4f interpolate_rotation(HeimBone* bone, float animation_time) {
    if (bone->num_rotations == 1) {
        HeimVec4f dest = heim_vec4f_normalize(bone->rotations[0].orientation);
        return dest;
    }
    int32_t p0_index = heim_bone_get_rotation_index(bone, animation_time);
    int32_t p1_index = p0_index + 1;
    float scale_factor = get_scale_factor(bone->rotations[p0_index].time_stamp, bone->rotations[p1_index].time_stamp, animation_time);

    HeimVec4f final_rotation = heim_quat_slerp(bone->rotations[p0_index].orientation, bone->rotations[p1_index].orientation, scale_factor);
    final_rotation = heim_vec4f_normalize(final_rotation);
    return final_rotation;
}

HeimVec3f interpolate_scaling(HeimBone* bone, float animation_time) {
    if(bone->num_scales == 1) {
        return bone->scales[0].scale;
    }
    int32_t p0_index = heim_bone_get_scale_index(bone, animation_time);
    int32_t p1_index = p0_index + 1;
    float scale_factor = get_scale_factor(bone->scales[p0_index].time_stamp, bone->scales[p1_index].time_stamp, animation_time);
    HeimVec3f final_scale = heim_vec3f_mix(bone->scales[p0_index].scale, bone->scales[p1_index].scale, scale_factor);
    return final_scale;
}
