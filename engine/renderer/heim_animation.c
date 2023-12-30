#include "renderer/heim_animation.h"

#include <assert.h>
#include <assimp/anim.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdint.h>
#include <stdio.h>

#include "core/utils/heim_vector.h"
#include "renderer/heim_skeletal_model.h"
#include "renderer/skeletal_utils/assimp_glm_helpers.h"
#include "renderer/skeletal_utils/heim_bone.h"

void read_missing_bones(HeimAnimation* animation, const struct aiAnimation* ai_animation, HeimSkeletalModel* model);
void read_heirarchy_data(assimp_node_data_t* assimp_node_data, const struct aiNode* src);
void free_heirarchy_data(assimp_node_data_t* assimp_node_data);

HeimAnimation* heim_animation_init(const struct aiScene* scene, struct aiAnimation* animation, HeimSkeletalModel* model) {
    HeimAnimation* anim = heim_malloc(sizeof(HeimAnimation), HEIM_MEMORY_TYPE_RENDERER, __FILE__, __LINE__);
    memset(anim, 0, sizeof(HeimAnimation));
    anim->duration = animation->mDuration;
    anim->ticks_per_second = animation->mTicksPerSecond;
    anim->name = heim_malloc(strlen(animation->mName.data) + 1, HEIM_MEMORY_TYPE_RENDERER, __FILE__, __LINE__);
    anim->bones = heim_vector_create(HeimBone*);
    strcpy(anim->name, animation->mName.data);
    read_heirarchy_data(&anim->root_node, scene->mRootNode);
    read_missing_bones(anim, animation, model);
    return anim;
}

void heim_animation_free(HeimAnimation* animation) {
    for (size_t i = 0; i < heim_vector_length(animation->bones); i++) {
        heim_bone_free(animation->bones[i]);
    }
    heim_vector_destroy(animation->bones);
    free_heirarchy_data(&animation->root_node);
    HEIM_FREE(animation->name, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(animation, HEIM_MEMORY_TYPE_RENDERER);
}

HeimBone* heim_animation_find_bone(HeimAnimation* animation, const char* name) {
    for (size_t i = 0; i < heim_vector_length(animation->bones); i++) {
        if (strcmp(animation->bones[i]->name, name) == 0) {
            return animation->bones[i];
        }
    }
    return NULL;
}

void read_missing_bones(HeimAnimation* animation, const struct aiAnimation* ai_animation, HeimSkeletalModel* model) {
    int32_t size = ai_animation->mNumChannels;
    heim_bone_info_t* bone_info_map = model->bone_info_map;
    int32_t bone_count = model->bone_counter;

    for (int i = 0; i < size; i++) {
        struct aiNodeAnim* channel = ai_animation->mChannels[i];
        const char* bone_name = strdup(channel->mNodeName.data);

        if (find_bone_index(bone_info_map, bone_name) == -1) {
            heim_bone_info_t info = {0};
            info.id = bone_count;
            info.name = HEIM_CALLOC(char, strlen(bone_name) + 1, HEIM_MEMORY_TYPE_RENDERER);
            strcpy(info.name, bone_name);
            heim_vector_push(bone_info_map, info);
            bone_count++;
        }
        int32_t bone_idx = find_bone_index(bone_info_map, bone_name);
        HeimBone* bone = heim_bone_init(channel->mNodeName.data, bone_info_map[bone_idx].id, channel);
        heim_vector_push(animation->bones, bone);
    }
    animation->bone_info_map = bone_info_map;
}

void read_heirarchy_data(assimp_node_data_t* dest, const struct aiNode* src) {
    assert(src);
    dest->name = heim_malloc(strlen(src->mName.data) + 1, HEIM_MEMORY_TYPE_RENDERER, __FILE__, __LINE__);
    strcpy(dest->name, src->mName.data);
    dest->transformation = convert_matrix_to_glm(&src->mTransformation);
    dest->children_count = src->mNumChildren;
    if (dest->children_count > 0) {
        dest->children = heim_vector_create(assimp_node_data_t);

        for (uint32_t i = 0; i < src->mNumChildren; i++) {
            assimp_node_data_t child = {0};
            read_heirarchy_data(&child, src->mChildren[i]);
            heim_vector_push(dest->children, child);
        }
    }
}

void free_heirarchy_data(assimp_node_data_t* assimp_node_data) {
    HEIM_FREE(assimp_node_data->name, HEIM_MEMORY_TYPE_RENDERER);
    if (assimp_node_data->children_count > 0) {
        for (size_t i = 0; i < heim_vector_length(assimp_node_data->children); i++) {
            free_heirarchy_data(&assimp_node_data->children[i]);
        }
        heim_vector_destroy(assimp_node_data->children);
    }
}
