#include "renderer/heim_skybox.h"

#include <GL/glew.h>
#include <stb/stb_image.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "math/heim_vec.h"

static HeimMat4 captureProjection;
static HeimMat4 captureViews[6];

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void render_cube() {
    // initialize (if necessary)
    if (cubeVAO == 0) {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   // top-left
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            // front face
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top-left
            // left face
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
                                                                 // right face
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // top-left
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     // top-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // bottom-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // bottom-right
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // bottom-left
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // top-left
            // bottom face
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // top-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
            // top face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void render_quad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // top-left
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
            1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  // top-right
            1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

HeimSkybox* heim_skybox_create(const char* path) {
    captureProjection = heim_mat4_perspective(90.0f, 1.0f, 0.1f, 10.0f);

    captureViews[0] =
        heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){1.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, -1.0f, 0.0f});
    captureViews[1] =
        heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){-1.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, -1.0f, 0.0f});
    captureViews[2] = heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, 1.0f, 0.0f}, (HeimVec3f){0.0f, 0.0f, 1.0f});
    captureViews[3] =
        heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, -1.0f, 0.0f}, (HeimVec3f){0.0f, 0.0f, -1.0f});
    captureViews[4] =
        heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, 0.0f, 1.0f}, (HeimVec3f){0.0f, -1.0f, 0.0f});
    captureViews[5] =
        heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, 0.0f, -1.0f}, (HeimVec3f){0.0f, -1.0f, 0.0f});

    HeimShader* equirectangularToCubemapShader = heim_shader_create();
    heim_shader_init(equirectangularToCubemapShader, "assets/shaders/cubemap.vert", "assets/shaders/equirect_to_cube.frag");

    HeimShader* irradianceShader = heim_shader_create();
    heim_shader_init(irradianceShader, "assets/shaders/cubemap.vert", "assets/shaders/irradiance_conv.frag");

    HeimShader* prefilterShader = heim_shader_create();
    heim_shader_init(prefilterShader, "assets/shaders/cubemap.vert", "assets/shaders/prefilter.frag");

    HeimShader* brdfShader = heim_shader_create();
    heim_shader_init(brdfShader, "assets/shaders/brdf.vert", "assets/shaders/brdf.frag");

    HeimShader* skyboxShader = heim_shader_create();
    heim_shader_init(skyboxShader, "assets/shaders/background.vert", "assets/shaders/background.frag");
    heim_shader_set_uniform1i(skyboxShader, "environmentMap", 0);

    HeimSkybox* skybox = HEIM_MALLOC(HeimSkybox, HEIM_MEMORY_TYPE_RENDERER);

    uint32_t captureFBO;
    uint32_t captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    stbi_set_flip_vertically_on_load(1);
    int32_t width, height, channels;

    // Load the HDR image
    float* data = stbi_loadf(path, &width, &height, &channels, 0);
    if (!data) {
        HEIM_LOG_ERROR("Failed to load HDR image: %s", path);
        return NULL;
    }

    uint32_t hdrTexture;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    glGenTextures(1, &skybox->env_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->env_cubemap);

    for (uint32_t i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    heim_shader_bind(equirectangularToCubemapShader);
    heim_shader_set_uniform1i(equirectangularToCubemapShader, "equirectangularMap", 0);
    heim_shader_set_uniform_mat4(equirectangularToCubemapShader, "projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (uint32_t i = 0; i < 6; i++) {
        heim_shader_set_uniform_mat4(equirectangularToCubemapShader, "view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox->env_cubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_cube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->env_cubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glGenTextures(1, &skybox->irradiance_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->irradiance_map);

    for (uint32_t i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    heim_shader_bind(irradianceShader);
    heim_shader_set_uniform1i(irradianceShader, "environmentMap", 0);
    heim_shader_set_uniform_mat4(irradianceShader, "projection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->env_cubemap);
    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    for (uint32_t i = 0; i < 6; i++) {
        heim_shader_set_uniform_mat4(irradianceShader, "view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox->irradiance_map, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_cube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &skybox->prefilter_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->prefilter_map);
    for (uint32_t i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    heim_shader_bind(prefilterShader);
    heim_shader_set_uniform1i(prefilterShader, "environmentMap", 0);
    heim_shader_set_uniform_mat4(prefilterShader, "projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->env_cubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    uint32_t maxMipLevels = 5;

    for (uint32_t mip = 0; mip < maxMipLevels; mip++) {
        uint32_t mipWidth = 128 * pow(0.5, mip);
        uint32_t mipHeight = 128 * pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        heim_shader_set_uniform1f(prefilterShader, "roughness", roughness);
        for (uint32_t i = 0; i < 6; i++) {
            heim_shader_set_uniform_mat4(prefilterShader, "view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox->prefilter_map,
                                   mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render_cube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &skybox->brdf_lut);

    glBindTexture(GL_TEXTURE_2D, skybox->brdf_lut);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, skybox->brdf_lut, 0);

    glViewport(0, 0, 512, 512);

    heim_shader_bind(brdfShader);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_quad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
    heim_shader_free(prefilterShader);
    heim_shader_free(irradianceShader);
    heim_shader_free(brdfShader);
    heim_shader_free(equirectangularToCubemapShader);

    skybox->background_shader = skyboxShader;

    return skybox;
}

void heim_skybox_destroy(HeimSkybox* skybox) {
    glDeleteTextures(1, &skybox->env_cubemap);
    glDeleteTextures(1, &skybox->irradiance_map);
    glDeleteTextures(1, &skybox->prefilter_map);
    glDeleteTextures(1, &skybox->brdf_lut);
    heim_shader_free(skybox->background_shader);
    HEIM_FREE(skybox, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_skybox_bind(HeimSkybox* skybox) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->irradiance_map);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->prefilter_map);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, skybox->brdf_lut);
}

void heim_skybox_render_background(HeimSkybox* skybox, HeimMat4 view, HeimMat4 projection) {
    heim_shader_bind(skybox->background_shader);
    heim_shader_set_uniform_mat4(skybox->background_shader, "view", view);
    heim_shader_set_uniform_mat4(skybox->background_shader, "projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->env_cubemap);
    heim_shader_set_uniform1i(skybox->background_shader, "environmentMap", 0);

    glDepthFunc(GL_LEQUAL);
    render_cube();
    glDepthFunc(GL_LESS);
}