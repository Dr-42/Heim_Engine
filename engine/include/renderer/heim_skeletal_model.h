#pragma once

#include <stdint.h>

#include "math/heim_mat.h"
#include "renderer/heim_skeletal_mesh.h"
#include "renderer/heim_texture.h"

typedef struct heim_bone_info_t {
    int32_t id;
    HeimMat4 offset;
    char* name;
} heim_bone_info_t;

typedef struct HeimSkeletalModel {
    HeimTexture *albedo;
    HeimTexture *normal;
    HeimTexture *specular;
    HeimTexture *roughness;
    HeimTexture *ao;

    HeimSkeletalMesh **meshes;
    heim_bone_info_t* bone_info_map;
    int32_t bone_counter;
} HeimSkeletalModel;

HeimSkeletalModel* heim_skeletal_model_create(const char* path);
void heim_skeletal_model_destroy(HeimSkeletalModel* model);

void heim_skeletal_model_set_albedo(HeimSkeletalModel* model, HeimTexture* albedo);
void heim_skeletal_model_set_normal(HeimSkeletalModel* model, HeimTexture* normal);
void heim_skeletal_model_set_specular(HeimSkeletalModel* model, HeimTexture* specular);
void heim_skeletal_model_set_roughness(HeimSkeletalModel* model, HeimTexture* roughness);
void heim_skeletal_model_set_ao(HeimSkeletalModel* model, HeimTexture* ao);

void heim_skeletal_model_draw(HeimSkeletalModel* model);

int32_t find_bone_index(heim_bone_info_t* bone_info_map, const char* name);

