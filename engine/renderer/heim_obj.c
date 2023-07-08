#include "renderer/heim_obj.h"

#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <time.h>

#include "core/heim_memory.h"

#define ASSIMP_FLAGS aiProcess_Triangulate | aiProcess_GenNormals

// Face structure
typedef struct HeimObjFace {
    uint64_t vertex_index[3];
    uint64_t normal_index[3];
    uint64_t uv_index[3];
} HeimObjFace;

HeimObj *heim_obj_load(const char *path) {
    HeimObj *obj = HEIM_MALLOC(HeimObj, HEIM_MEMORY_TYPE_RENDERER);

    // Load the model with assimp
    const struct aiScene *scene = aiImportFile(path, ASSIMP_FLAGS);

    if (!scene) {
        HEIM_LOG_ERROR("Failed to load model %s", path);
        HEIM_LOG_ERROR("%s", aiGetErrorString());
        return NULL;
    }

    // Get the first mesh in the scene
    const struct aiMesh *mesh = scene->mMeshes[0];

    // Get the vertices, normals and uv coordinates
    HeimVec3f *vertices = HEIM_CALLOC(HeimVec3f, mesh->mNumVertices, HEIM_MEMORY_TYPE_RENDERER);
    HeimVec3f *normals = HEIM_CALLOC(HeimVec3f, mesh->mNumVertices, HEIM_MEMORY_TYPE_RENDERER);
    HeimVec2f *uvs = HEIM_CALLOC(HeimVec2f, mesh->mNumVertices, HEIM_MEMORY_TYPE_RENDERER);

    for (uint64_t i = 0; i < mesh->mNumVertices; i++) {
        vertices[i].x = mesh->mVertices[i].x;
        vertices[i].y = mesh->mVertices[i].y;
        vertices[i].z = mesh->mVertices[i].z;

        normals[i].x = mesh->mNormals[i].x;
        normals[i].y = mesh->mNormals[i].y;
        normals[i].z = mesh->mNormals[i].z;

        uvs[i].x = mesh->mTextureCoords[0][i].x;
        uvs[i].y = mesh->mTextureCoords[0][i].y;
    }

    // Fill in the vertices data
    obj->face_count = mesh->mNumFaces;
    obj->vertices_data = HEIM_CALLOC(HeimVertex, obj->face_count * 3, HEIM_MEMORY_TYPE_RENDERER);

    for (uint64_t i = 0; i < mesh->mNumFaces; i++) {
        struct aiFace face = mesh->mFaces[i];
        for (uint64_t j = 0; j < face.mNumIndices; j++) {
            obj->vertices_data[i * 3 + j].position = vertices[face.mIndices[j]];
            obj->vertices_data[i * 3 + j].normal = normals[face.mIndices[j]];
            obj->vertices_data[i * 3 + j].uv = uvs[face.mIndices[j]];
        }
    }

    HEIM_FREE(vertices, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(normals, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(uvs, HEIM_MEMORY_TYPE_RENDERER);
    // Free the scene
    aiReleaseImport(scene);

    glGenVertexArrays(1, &obj->vao);
    glBindVertexArray(obj->vao);
    // Generate the VBO for vertices
    glGenBuffers(1, &obj->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
    glBufferData(GL_ARRAY_BUFFER, obj->face_count * sizeof(HeimVertex) * 3, obj->vertices_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HeimVertex), (void *)offsetof(HeimVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HeimVertex), (void *)offsetof(HeimVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(HeimVertex), (void *)offsetof(HeimVertex, uv));
    return obj;
}

void heim_obj_free(HeimObj *obj) {
    HEIM_FREE(obj->vertices_data, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(obj, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_obj_render(HeimObj *obj) {
    // heim_shader_bind(obj->shader);
    glBindVertexArray(obj->vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);

    // Bind the texture
    // Draw the object
    glDrawArrays(GL_TRIANGLES, 0, obj->face_count * 3);

    // Unbind the VAO and VBOs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}