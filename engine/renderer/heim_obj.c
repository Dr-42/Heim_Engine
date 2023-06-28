#include "renderer/heim_obj.h"

#include <GL/glew.h>

#include "core/heim_memory.h"

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

    obj->vertices = HEIM_CALLOC(HeimVec3f, obj->vertex_count, HEIM_MEMORY_TYPE_RENDERER);
    obj->normals = HEIM_CALLOC(HeimVec3f, obj->normal_count, HEIM_MEMORY_TYPE_RENDERER);
    obj->uvs = HEIM_CALLOC(HeimVec2f, obj->uv_count, HEIM_MEMORY_TYPE_RENDERER);
    obj->faces = HEIM_CALLOC(HeimObjFace, obj->face_count, HEIM_MEMORY_TYPE_RENDERER);

    fseek(file, 0, SEEK_SET);

    uint64_t vertex_index = 0;
    uint64_t face_index = 0;
    uint64_t normal_index = 0;
    uint64_t uv_index = 0;

    heim_memzero(line, sizeof(line));

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &obj->vertices[vertex_index].x, &obj->vertices[vertex_index].y, &obj->vertices[vertex_index].z);
            vertex_index++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            sscanf(line, "f %llu/%llu/%llu %llu/%llu/%llu %llu/%llu/%llu",
                   &obj->faces[face_index].vertex_index[0], &obj->faces[face_index].uv_index[0], &obj->faces[face_index].normal_index[0],
                   &obj->faces[face_index].vertex_index[1], &obj->faces[face_index].uv_index[1], &obj->faces[face_index].normal_index[1],
                   &obj->faces[face_index].vertex_index[2], &obj->faces[face_index].uv_index[2], &obj->faces[face_index].normal_index[2]);

            obj->faces[face_index].vertex_index[0]--;
            obj->faces[face_index].vertex_index[1]--;
            obj->faces[face_index].vertex_index[2]--;

            obj->faces[face_index].uv_index[0]--;
            obj->faces[face_index].uv_index[1]--;
            obj->faces[face_index].uv_index[2]--;

            obj->faces[face_index].normal_index[0]--;
            obj->faces[face_index].normal_index[1]--;
            obj->faces[face_index].normal_index[2]--;
            face_index++;
        } else if (line[0] == 'v' && line[1] == 'n') {
            sscanf(line, "vn %f %f %f", &obj->normals[normal_index].x, &obj->normals[normal_index].y, &obj->normals[normal_index].z);
            normal_index++;
        } else if (line[0] == 'v' && line[1] == 't') {
            if (sscanf(line, "vt %f %f", &obj->uvs[uv_index].x, &obj->uvs[uv_index].y) != 2) {
                HEIM_LOG_WARN("Failed to parse uv: %s", line);
            }
            uv_index++;
        }

        heim_memzero(line, sizeof(line));
    }

    obj->elements = HEIM_CALLOC(uint32_t, obj->face_count * 3, HEIM_MEMORY_TYPE_RENDERER);

    for (uint64_t i = 0; i < obj->face_count; i++) {
        obj->elements[i * 3 + 0] = obj->faces[i].vertex_index[0];
        obj->elements[i * 3 + 1] = obj->faces[i].vertex_index[1];
        obj->elements[i * 3 + 2] = obj->faces[i].vertex_index[2];
    }

    fclose(file);

    obj->vertices_data = HEIM_CALLOC(HeimVertex, obj->face_count * 3, HEIM_MEMORY_TYPE_RENDERER);

    // Fill in the heim vertices_data array according to the faces
    // Each Heim vertex has a position, normal and uv
    for (uint64_t i = 0; i < obj->face_count; i++) {
        obj->vertices_data[i * 3 + 0].position = obj->vertices[obj->faces[i].vertex_index[0]];
        obj->vertices_data[i * 3 + 1].position = obj->vertices[obj->faces[i].vertex_index[1]];
        obj->vertices_data[i * 3 + 2].position = obj->vertices[obj->faces[i].vertex_index[2]];

        obj->vertices_data[i * 3 + 0].normal = obj->normals[obj->faces[i].normal_index[0]];
        obj->vertices_data[i * 3 + 1].normal = obj->normals[obj->faces[i].normal_index[1]];
        obj->vertices_data[i * 3 + 2].normal = obj->normals[obj->faces[i].normal_index[2]];

        obj->vertices_data[i * 3 + 0].uv = obj->uvs[obj->faces[i].uv_index[0]];
        obj->vertices_data[i * 3 + 1].uv = obj->uvs[obj->faces[i].uv_index[1]];
        obj->vertices_data[i * 3 + 2].uv = obj->uvs[obj->faces[i].uv_index[2]];
    }

    return obj;
}

void heim_obj_free(HeimObj *obj) {
    HEIM_FREE(obj->vertices, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(obj->normals, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(obj->uvs, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(obj->faces, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(obj, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_obj_render(HeimObj *obj) {
    // Generate and bind the VAO
    glGenVertexArrays(1, &obj->vao);
    glBindVertexArray(obj->vao);

    // Generate the VBO for vertices
    /*
    glGenBuffers(1, &obj->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
    glBufferData(GL_ARRAY_BUFFER, obj->vertex_count * sizeof(HeimVec3f), obj->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HeimVec3f), (void *)0);

    // Generate the VBO for normals
    uint32_t normal_vbo;
    glGenBuffers(1, &normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, obj->normal_count * sizeof(HeimVec3f), obj->normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HeimVec3f), (void *)0);

    // Generate the VBO for UVs
    uint32_t uv_vbo;
    glGenBuffers(1, &uv_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glBufferData(GL_ARRAY_BUFFER, obj->uv_count * sizeof(HeimVec2f), obj->uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(HeimVec2f), (void *)0);

    // Generate the EBO for faces
    glGenBuffers(1, &obj->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->face_count * 3 * sizeof(uint32_t), obj->elements, GL_STATIC_DRAW);
    */

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

    // Draw the object
    // glDrawElements(GL_TRIANGLES, obj->face_count * 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, obj->face_count * 3);

    // Unbind the VAO and VBOs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete the VBOs
    // glDeleteBuffers(1, &normal_vbo);
    // glDeleteBuffers(1, &uv_vbo);
}