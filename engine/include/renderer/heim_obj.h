#ifndef HEIM_OBJ_H
#define HEIM_OBJ_H

#include <stdint.h>

#include "math/heim_mat.h"

// Face structure
typedef struct HeimObjFace {
    uint64_t vertex_index[3];
    uint64_t normal_index[3];
    uint64_t uv_index[3];
} HeimObjFace;

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

    HeimVec3f *vertices;
    HeimVec3f *normals;
    HeimVec2f *uvs;
    HeimObjFace *faces;

    uint32_t *elements;

    HeimVertex *vertices_data;

    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
} HeimObj;

HeimObj *heim_obj_load(const char *path);
void heim_obj_free(HeimObj *obj);
void heim_obj_render(HeimObj *obj);

#endif  // HEIM_OBJ_H
