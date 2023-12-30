#pragma once

#include "renderer/heim_mesh.h"
#include "renderer/heim_texture.h"

typedef struct HeimModel {
    HeimTexture *albedo;
    HeimTexture *normal;
    HeimTexture *specular;
    HeimTexture *roughness;
    HeimTexture *ao;

    HeimMesh **meshes;
} HeimModel;

HeimModel* heim_model_create(const char* path);
void heim_model_destroy(HeimModel* model);

void heim_model_set_albedo(HeimModel* model, HeimTexture* albedo);
void heim_model_set_normal(HeimModel* model, HeimTexture* normal);
void heim_model_set_specular(HeimModel* model, HeimTexture* specular);
void heim_model_set_roughness(HeimModel* model, HeimTexture* roughness);
void heim_model_set_ao(HeimModel* model, HeimTexture* ao);

void heim_model_draw(HeimModel* model);
