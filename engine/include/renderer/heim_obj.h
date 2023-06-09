#pragma once

#include <stdint.h>

#include "math/heim_mat.h"
#include "renderer/heim_shader.h"
#include "renderer/heim_texture.h"

typedef struct HeimVertex {
    HeimVec3f position;
    HeimVec3f normal;
    HeimVec2f uv;
} HeimVertex;

// Obj model
typedef struct HeimObj {
    uint64_t vertex_count;
    uint64_t normal_count;
    uint64_t uv_count;
    uint64_t face_count;

    HeimVertex *vertices_data;

    uint32_t vao;
    uint32_t vbo;
} HeimObj;

HeimObj *heim_obj_load(const char *path);
void heim_obj_free(HeimObj *obj);
void heim_obj_render(HeimObj *obj);
