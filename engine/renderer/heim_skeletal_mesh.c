#include "renderer/heim_skeletal_mesh.h"
#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"

#include <string.h>

#define todo() { fprintf(stderr, "TODO: %s:%d%d", __FILE__, __LINE__, 0); }

HeimSkeletalMesh* heim_skeletal_mesh_create(HeimSkeletalMeshVertex* vertices, uint32_t* indices){
    HeimSkeletalMesh* mesh = HEIM_MALLOC(HeimSkeletalMesh, HEIM_MEMORY_TYPE_RENDERER);
    memset(mesh, 0, sizeof(HeimSkeletalMesh));
    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->num_indices = heim_vector_length(mesh->indices);
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, heim_vector_length(mesh->vertices) * sizeof(HeimSkeletalMeshVertex), mesh->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, heim_vector_length(mesh->indices) * sizeof(uint32_t), mesh->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HeimSkeletalMeshVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HeimSkeletalMeshVertex), (void*)offsetof(HeimSkeletalMeshVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(HeimSkeletalMeshVertex), (void*)offsetof(HeimSkeletalMeshVertex, tex_coords));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(HeimSkeletalMeshVertex), (void*)offsetof(HeimSkeletalMeshVertex, ids));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(HeimSkeletalMeshVertex), (void*)offsetof(HeimSkeletalMeshVertex, weights));

    glBindVertexArray(0);
    return mesh;
}

void heim_skeletal_mesh_destroy(HeimSkeletalMesh* mesh){
    HEIM_FREE(mesh, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_skeletal_mesh_draw(HeimSkeletalMesh* mesh){
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

