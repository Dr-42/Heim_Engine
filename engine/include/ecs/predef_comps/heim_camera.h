#pragma once

#include <stdbool.h>

#include "math/heim_vec.h"
#include "renderer/heim_texture.h"

typedef struct HeimCamera {
    float fov;
    float aspect;
    float near;
    float far;

    float yaw;
    float pitch;

    HeimVec3f front;
    HeimVec3f up;

    bool render_to_texture;
    HeimTexture* texture;
    uint32_t framebuffer;
    uint32_t depthbuffer;

    HeimVec4f clear_color;
} HeimCamera;

HeimCamera* heim_camera_new(float fov, float aspect, float near, float far, bool render_to_texture, uint32_t width, uint32_t height);
void heim_camera_free(HeimCamera* camera);

void heim_camera_bind(HeimCamera* camera);
void heim_camera_unbind(HeimCamera* camera);

void heim_camera_clear(HeimCamera* camera);