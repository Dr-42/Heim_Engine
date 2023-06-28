#include "renderer/heim_obj.h"

#include <GL/glew.h>

#include "core/heim_memory.h"

// Face structure
typedef struct HeimObjFace {
    uint64_t vertex_index[3];
    uint64_t normal_index[3];
    uint64_t uv_index[3];
} HeimObjFace;

HeimObj *heim_obj_load(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        HEIM_LOG_ERROR("Failed to open file: %s", path);
        return NULL;
    }

    HeimObj *obj = HEIM_CALLOC(HeimObj, 1, HEIM_MEMORY_TYPE_RENDERER);
    heim_memzero(obj, sizeof(HeimObj));

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            obj->vertex_count++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            obj->face_count++;
        } else if (line[0] == 'v' && line[1] == 'n') {
            obj->normal_count++;
        } else if (line[0] == 'v' && line[1] == 't') {
            obj->uv_count++;
        }
    }

    HeimVec3f *vertices = HEIM_CALLOC(HeimVec3f, obj->vertex_count, HEIM_MEMORY_TYPE_RENDERER);
    HeimVec3f *normals = HEIM_CALLOC(HeimVec3f, obj->normal_count, HEIM_MEMORY_TYPE_RENDERER);
    HeimVec2f *uvs = HEIM_CALLOC(HeimVec2f, obj->uv_count, HEIM_MEMORY_TYPE_RENDERER);
    HeimObjFace *faces = HEIM_CALLOC(HeimObjFace, obj->face_count, HEIM_MEMORY_TYPE_RENDERER);

    fseek(file, 0, SEEK_SET);

    uint64_t vertex_index = 0;
    uint64_t face_index = 0;
    uint64_t normal_index = 0;
    uint64_t uv_index = 0;

    heim_memzero(line, sizeof(line));

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &vertices[vertex_index].x, &vertices[vertex_index].y, &vertices[vertex_index].z);
            vertex_index++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            sscanf(line, "f %llu/%llu/%llu %llu/%llu/%llu %llu/%llu/%llu",
                   &faces[face_index].vertex_index[0], &faces[face_index].uv_index[0], &faces[face_index].normal_index[0],
                   &faces[face_index].vertex_index[1], &faces[face_index].uv_index[1], &faces[face_index].normal_index[1],
                   &faces[face_index].vertex_index[2], &faces[face_index].uv_index[2], &faces[face_index].normal_index[2]);

            faces[face_index].vertex_index[0]--;
            faces[face_index].vertex_index[1]--;
            faces[face_index].vertex_index[2]--;

            faces[face_index].uv_index[0]--;
            faces[face_index].uv_index[1]--;
            faces[face_index].uv_index[2]--;

            faces[face_index].normal_index[0]--;
            faces[face_index].normal_index[1]--;
            faces[face_index].normal_index[2]--;
            face_index++;
        } else if (line[0] == 'v' && line[1] == 'n') {
            sscanf(line, "vn %f %f %f", &normals[normal_index].x, &normals[normal_index].y, &normals[normal_index].z);
            normal_index++;
        } else if (line[0] == 'v' && line[1] == 't') {
            if (sscanf(line, "vt %f %f", &uvs[uv_index].x, &uvs[uv_index].y) != 2) {
                HEIM_LOG_WARN("Failed to parse uv: %s", line);
            }
            uv_index++;
        }

        heim_memzero(line, sizeof(line));
    }

    fclose(file);

    obj->vertices_data = HEIM_CALLOC(HeimVertex, obj->face_count * 3, HEIM_MEMORY_TYPE_RENDERER);

    // Fill in the heim vertices_data array according to the faces
    // Each Heim vertex has a position, normal and uv
    for (uint64_t i = 0; i < obj->face_count; i++) {
        obj->vertices_data[i * 3 + 0].position = vertices[faces[i].vertex_index[0]];
        obj->vertices_data[i * 3 + 1].position = vertices[faces[i].vertex_index[1]];
        obj->vertices_data[i * 3 + 2].position = vertices[faces[i].vertex_index[2]];

        obj->vertices_data[i * 3 + 0].normal = normals[faces[i].normal_index[0]];
        obj->vertices_data[i * 3 + 1].normal = normals[faces[i].normal_index[1]];
        obj->vertices_data[i * 3 + 2].normal = normals[faces[i].normal_index[2]];

        obj->vertices_data[i * 3 + 0].uv = uvs[faces[i].uv_index[0]];
        obj->vertices_data[i * 3 + 1].uv = uvs[faces[i].uv_index[1]];
        obj->vertices_data[i * 3 + 2].uv = uvs[faces[i].uv_index[2]];
    }

    HEIM_FREE(vertices, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(faces, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(normals, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(uvs, HEIM_MEMORY_TYPE_RENDERER);

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
    glBindVertexArray(obj->vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);

    // Draw the object
    glDrawArrays(GL_TRIANGLES, 0, obj->face_count * 3);

    // Unbind the VAO and VBOs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}