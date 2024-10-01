#pragma once

#include <GL/glew.h>
#include "math/heim_vec.h"
#include "renderer/heim_texture.h"
#include <stdint.h>

#define MAX_BONE_INFLUENCE 4

typedef struct HeimSkeletalMeshVertex {
    HeimVec3f position;
    HeimVec3f normal;
    HeimVec2f tex_coords;
    int32_t ids[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
} HeimSkeletalMeshVertex;

typedef struct HeimSkeletalMesh {
    HeimSkeletalMeshVertex* vertices;
    uint32_t* indices;
    uint32_t num_indices;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} HeimSkeletalMesh;

HeimSkeletalMesh* heim_skeletal_mesh_create(HeimSkeletalMeshVertex* vertices, uint32_t* indices);
void heim_skeletal_mesh_destroy(HeimSkeletalMesh* mesh);

void heim_skeletal_mesh_draw(HeimSkeletalMesh* mesh);
