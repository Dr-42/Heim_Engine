#pragma once

#include <GL/glew.h>
#include "math/heim_vec.h"
#include <stdint.h>

#define MAX_BONE_INFLUENCE 4

typedef struct HeimMeshVertex {
    HeimVec3f position;
    HeimVec3f normal;
    HeimVec2f tex_coords;
} HeimMeshVertex;

typedef struct HeimMesh {
    HeimMeshVertex* vertices;
    uint32_t* indices;
    uint32_t num_indices;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} HeimMesh;

HeimMesh* heim_mesh_create(HeimMeshVertex* vertices, uint32_t* indices);
void heim_mesh_destroy(HeimMesh* mesh);

void heim_mesh_draw(HeimMesh* mesh);
