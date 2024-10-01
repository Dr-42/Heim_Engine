#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string.h>
#include <assert.h>

#include "renderer/heim_skeletal_model.h"
#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"
#include "renderer/heim_skeletal_mesh.h"
#include "renderer/skeletal_utils/assimp_glm_helpers.h"

static void load_model(HeimSkeletalModel* model, const char* path);
static void process_node(HeimSkeletalModel* model, struct aiNode* node, const struct aiScene* scene);
static void set_vertex_bone_data_to_default(HeimSkeletalMeshVertex* vertex);
static HeimSkeletalMesh* process_mesh(HeimSkeletalModel* model, struct aiMesh* mesh);
static void set_vertex_bone_data(HeimSkeletalMeshVertex* vertex, int bone_id, float weight);
static void extract_bone_weight_for_vertices(HeimSkeletalModel* model, HeimSkeletalMeshVertex* vertices, struct aiMesh* mesh);

HeimSkeletalModel* heim_skeletal_model_create(const char* path) {
    HeimSkeletalModel* model = HEIM_MALLOC(HeimSkeletalModel, HEIM_MEMORY_TYPE_RENDERER);
    memset(model, 0, sizeof(HeimSkeletalModel));
    model->meshes = heim_vector_create(HeimSkeletalMesh*);
    model->bone_info_map = heim_vector_create(heim_bone_info_t);
    load_model(model, path);
    return model;
}

void heim_skeletal_model_destroy(HeimSkeletalModel* model) {
    for (size_t i = 0; i < heim_vector_length(model->meshes); i++) {
        heim_vector_destroy(model->meshes[i]->vertices);
        heim_vector_destroy(model->meshes[i]->indices);
        heim_skeletal_mesh_destroy(model->meshes[i]);
    }
    heim_vector_destroy(model->meshes);
    for (size_t i = 0; i < heim_vector_length(model->bone_info_map); i++) {
        HEIM_FREE(model->bone_info_map[i].name, HEIM_MEMORY_TYPE_RENDERER);
    }
    heim_vector_destroy(model->bone_info_map);
    HEIM_FREE(model, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_skeletal_model_set_albedo(HeimSkeletalModel* model, HeimTexture* albedo) { model->albedo = albedo; }

void heim_skeletal_model_set_normal(HeimSkeletalModel* model, HeimTexture* normal) { model->normal = normal; }

void heim_skeletal_model_set_specular(HeimSkeletalModel* model, HeimTexture* specular) { model->specular = specular; }

void heim_skeletal_model_set_roughness(HeimSkeletalModel* model, HeimTexture* roughness) { model->roughness = roughness; }

void heim_skeletal_model_set_ao(HeimSkeletalModel* model, HeimTexture* ao) { model->ao = ao; }

void heim_skeletal_model_draw(HeimSkeletalModel* model) {
    for (size_t i = 0; i < heim_vector_length(model->meshes); i++) {
        heim_skeletal_mesh_draw(model->meshes[i]);
    }
}

void load_model(HeimSkeletalModel* model, const char* path) {
    const struct aiScene* scene =
        aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fprintf(stderr, "ERROR::ASSIMP::%s\n", aiGetErrorString());
        return;
    }
    process_node(model, scene->mRootNode, scene);
    aiReleaseImport(scene);
}

void process_node(HeimSkeletalModel* model, struct aiNode* node, const struct aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        HeimSkeletalMesh* m = process_mesh(model, mesh);
        heim_vector_push(model->meshes, m);
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        process_node(model, node->mChildren[i], scene);
    }
}

void set_vertex_bone_data_to_default(HeimSkeletalMeshVertex* vertex) {
    for (size_t i = 0; i < MAX_BONE_INFLUENCE; i++) {
        vertex->ids[i] = -1;
        vertex->weights[i] = 0.0f;
    }
}

HeimSkeletalMesh* process_mesh(HeimSkeletalModel* model, struct aiMesh* mesh) {
    HeimSkeletalMeshVertex* vertices = heim_vector_create(HeimSkeletalMeshVertex);
    uint32_t* indices = heim_vector_create(uint32_t);

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        HeimSkeletalMeshVertex vertex;
        set_vertex_bone_data_to_default(&vertex);
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

    extract_bone_weight_for_vertices(model, vertices, mesh);
    HeimSkeletalMesh* ret_mesh = heim_skeletal_mesh_create(vertices, indices);
    return ret_mesh;
}

void set_vertex_bone_data(HeimSkeletalMeshVertex* vertex, int bone_id, float weight) {
    for (size_t i = 0; i < MAX_BONE_INFLUENCE; i++) {
        if (vertex->ids[i] < 0.0f) {
            vertex->ids[i] = bone_id;
            vertex->weights[i] = weight;
            return;
        }
    }
}

int32_t find_bone_index(heim_bone_info_t* bone_info_map, const char* name) {
    for (int32_t i = 0; i < (int32_t)heim_vector_length(bone_info_map); i++) {
        if (strcmp(bone_info_map[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void extract_bone_weight_for_vertices(HeimSkeletalModel* model, HeimSkeletalMeshVertex* vertices, struct aiMesh* mesh) {
    int32_t bone_count = model->bone_counter;

    for (uint32_t bone_idx = 0; bone_idx < mesh->mNumBones; bone_idx++) {
        int32_t bone_id = -1;
        char* bone_name = strdup(mesh->mBones[bone_idx]->mName.data);
        if (find_bone_index(model->bone_info_map, bone_name) == -1) {
            heim_bone_info_t info = {0};
            info.id = bone_count;
            info.offset = convert_matrix_to_glm(&mesh->mBones[bone_idx]->mOffsetMatrix);
            // shput(model->bone_info_map, bone_name, info);
            info.name = HEIM_CALLOC(char, strlen(bone_name) + 1, HEIM_MEMORY_TYPE_RENDERER);
            strcpy(info.name, bone_name);
            heim_vector_push(model->bone_info_map, info);
            bone_id = bone_count;
            bone_count++;
        } else {
            int32_t bone_idx = find_bone_index(model->bone_info_map, bone_name);
            bone_id = model->bone_info_map[bone_idx].id;
        }
        assert(bone_id != -1);
        struct aiVertexWeight* weights = mesh->mBones[bone_idx]->mWeights;
        uint32_t num_weights = mesh->mBones[bone_idx]->mNumWeights;

        for (uint32_t weight_idx = 0; weight_idx < num_weights; weight_idx++) {
            uint32_t vertex_id = weights[weight_idx].mVertexId;
            float weight = weights[weight_idx].mWeight;
            assert(vertex_id < heim_vector_length(vertices));
            set_vertex_bone_data(&vertices[vertex_id], bone_id, weight);
        }
    }
}
