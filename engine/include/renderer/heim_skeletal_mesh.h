#pragma once

#include "GL/glew.h"
#include "renderer/heim_shader.h"
#include "math/heim_vec.h"
#include "renderer/heim_texture.h"
#include <stdint.h>

#define MAX_BONE_INFLUENCE 4

typedef struct HeimSkeletalMeshVertex {
    HeimVec3f position;
    HeimVec3f normal;
    HeimVec2f tex_coords;
    HeimVec3f tangent;
    HeimVec3f bitangent;
    int32_t ids[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
} HeimSkeletalMeshVertex;

typedef struct HeimSkeletalMesh {
    HeimSkeletalMeshVertex *vertices;
    uint32_t *indices;

    HeimTexture *albedo;
    HeimTexture *normal;
    HeimTexture *specular;
    HeimTexture *roughness;
    HeimTexture *ao;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} HeimSkeletalMesh;

HeimSkeletalMesh* heim_skeletal_mesh_create(HeimSkeletalMeshVertex* vertices, uint32_t* indices);
void heim_skeletal_mesh_destroy(HeimSkeletalMesh* mesh);

void heim_skeletal_mesh_set_albdeo(HeimSkeletalMesh* mesh, HeimTexture* texture);
void heim_skeletal_mesh_set_normal(HeimSkeletalMesh* mesh, HeimTexture* texture);
void heim_skeletal_mesh_set_specular(HeimSkeletalMesh* mesh, HeimTexture* texture);
void heim_skeletal_mesh_set_roughness(HeimSkeletalMesh* mesh, HeimTexture* texture);
void heim_skeletal_mesh_set_ao(HeimSkeletalMesh* mesh, HeimTexture* texture);

void heim_skeletal_mesh_draw(HeimSkeletalMesh* mesh, HeimShader* shader);
