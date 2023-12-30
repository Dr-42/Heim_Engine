#include "renderer/heim_model.h"
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string.h>

#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"
#include "renderer/skeletal_utils/assimp_glm_helpers.h"

static void load_model(HeimModel* model, const char* path);
static void process_node(HeimModel* model, struct aiNode* node, const struct aiScene* scene);
static HeimMesh* process_mesh(struct aiMesh* mesh);

HeimModel* heim_model_create(const char* path, bool gamma_corrected){
    HeimModel* model = HEIM_MALLOC(HeimModel, HEIM_MEMORY_TYPE_RENDERER);
    memset(model, 0, sizeof(HeimModel));
    model->gamma_corrected = gamma_corrected;
    model->meshes = heim_vector_create(HeimMesh*);
    load_model(model, path);
    return model;
}

void heim_model_destroy(HeimModel* model){
    for (size_t i = 0; i < heim_vector_length(model->meshes); i++) {
        heim_vector_destroy(model->meshes[i]->vertices);
        heim_vector_destroy(model->meshes[i]->indices);
        heim_mesh_destroy(model->meshes[i]);
    }
    heim_vector_destroy(model->meshes);
    HEIM_FREE(model, HEIM_MEMORY_TYPE_RENDERER);
}


void heim_model_set_albedo(HeimModel* model, HeimTexture* albedo){
    model->albedo = albedo;
}

void heim_model_set_normal(HeimModel* model, HeimTexture* normal){
    model->normal = normal;
}

void heim_model_set_specular(HeimModel* model, HeimTexture* specular){
    model->specular = specular;
}

void heim_model_set_roughness(HeimModel* model, HeimTexture* roughness){
    model->roughness = roughness;
}

void heim_model_set_ao(HeimModel* model, HeimTexture* ao){
    model->ao = ao;
}


void heim_model_draw(HeimModel* model){
    for (size_t i = 0; i < heim_vector_length(model->meshes); i++) {
        heim_mesh_draw(model->meshes[i]);
    }
}

void load_model(HeimModel* model, const char* path) {
    const struct aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fprintf(stderr, "ERROR::ASSIMP::%s\n", aiGetErrorString());
        return;
    }
    process_node(model, scene->mRootNode, scene);
    aiReleaseImport(scene);
}

void process_node(HeimModel* model, struct aiNode* node, const struct aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        HeimMesh* m = process_mesh(mesh);
        heim_vector_push(model->meshes, m);
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        process_node(model, node->mChildren[i], scene);
    }
}

HeimMesh* process_mesh(struct aiMesh* mesh) {
    HeimMeshVertex* vertices = heim_vector_create(HeimMeshVertex);
    uint32_t* indices = heim_vector_create(uint32_t);

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        HeimMeshVertex vertex;
        vertex.position = get_glm_vec(&mesh->mVertices[i]);
        vertex.normal = get_glm_vec(&mesh->mNormals[i]);
        if (mesh->mTextureCoords[0]) {
            vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.tex_coords.x = 0.0f;
            vertex.tex_coords.y = 0.0f;
        }
        heim_vector_push(vertices, vertex);
    }
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        struct aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            heim_vector_push(indices, face.mIndices[j]);
        }
    }
    HeimMesh* ret_mesh = heim_mesh_create(vertices, indices);
    return ret_mesh;
}
