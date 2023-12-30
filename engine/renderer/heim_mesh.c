#include "renderer/heim_mesh.h"
#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"

#include <string.h>

#define todo() { fprintf(stderr, "TODO: %s:%d%d", __FILE__, __LINE__, 0); }

HeimMesh* heim_mesh_create(HeimMeshVertex* vertices, uint32_t* indices){
    HeimMesh* mesh = HEIM_MALLOC(HeimMesh, HEIM_MEMORY_TYPE_RENDERER);
    memset(mesh, 0, sizeof(HeimMesh));
    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->num_indices = heim_vector_length(mesh->indices);
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, heim_vector_length(mesh->vertices) * sizeof(HeimMeshVertex), mesh->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, heim_vector_length(mesh->indices) * sizeof(uint32_t), mesh->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HeimMeshVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HeimMeshVertex), (void*)offsetof(HeimMeshVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(HeimMeshVertex), (void*)offsetof(HeimMeshVertex, tex_coords));

    glBindVertexArray(0);
    return mesh;
}

void heim_mesh_destroy(HeimMesh* mesh){
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    HEIM_FREE(mesh, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_mesh_draw(HeimMesh* mesh){
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

