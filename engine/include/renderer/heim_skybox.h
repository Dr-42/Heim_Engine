#pragma once

#include <stdint.h>

#include "math/heim_mat.h"
#include "renderer/heim_shader.h"

typedef struct HeimSkybox {
    uint32_t id;
    uint32_t width;
    uint32_t height;
    uint32_t channels;

    uint32_t irradiance_map;
    uint32_t prefilter_map;
    uint32_t brdf_lut;
    uint32_t env_cubemap;

    HeimShader* background_shader;
} HeimSkybox;

HeimSkybox* heim_skybox_create(const char* path);
void heim_skybox_destroy(HeimSkybox* skybox);

// Binds the skybox to the given texture slots
// The slots should be an array of 3 elements
void heim_skybox_bind(HeimSkybox* skybox);

void heim_skybox_render_background(HeimSkybox* skybox, HeimMat4 view, HeimMat4 projection);
