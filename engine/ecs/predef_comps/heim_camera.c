#include "ecs/predef_comps/heim_camera.h"

#include <GL/glew.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"

HeimCamera* heim_camera_new(float fov, float aspect, float near, float far, bool render_to_texture, uint32_t width, uint32_t height) {
    HeimCamera* camera = HEIM_MALLOC(HeimCamera, HEIM_MEMORY_TYPE_ECS);
    camera->fov = fov;
    camera->aspect = aspect;
    camera->near = near;
    camera->far = far;

    camera->yaw = 0.0f;
    camera->pitch = 0.0f;

    camera->front = (HeimVec3f){0.0f, 0.0f, -1.0f};
    camera->up = (HeimVec3f){0.0f, 1.0f, 0.0f};

    camera->render_to_texture = render_to_texture;

    if (render_to_texture) {
        camera->texture = heim_texture_create_empty_slot(width, height, 4);
        camera->framebuffer = 0;
        camera->depthbuffer = 0;

        glGenFramebuffers(1, &camera->framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, camera->framebuffer);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, camera->texture->id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenRenderbuffers(1, &camera->depthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, camera->depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, camera->texture->width, camera->texture->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, camera->depthbuffer);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, camera->texture->id, 0);
        GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, draw_buffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            HEIM_LOG_ERROR("Framebuffer not complete!");
        }
    }

    return camera;
}
void heim_camera_free(HeimCamera* camera) {
    if (camera->render_to_texture) {
        glDeleteFramebuffers(1, &camera->framebuffer);
        glDeleteRenderbuffers(1, &camera->depthbuffer);
    }
    heim_texture_free(camera->texture);
    HEIM_FREE(camera, HEIM_MEMORY_TYPE_ECS);
}

void heim_camera_bind(HeimCamera* camera) {
    if (camera->render_to_texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, camera->framebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, camera->depthbuffer);
        glViewport(0, 0, camera->texture->width, camera->texture->height);
    }
}

void heim_camera_unbind(HeimCamera* camera) {
    if (camera->render_to_texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glViewport(0, 0, camera->texture->width, camera->texture->height);
    }
}

void heim_camera_clear(HeimCamera* camera) {
    if (camera->render_to_texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, camera->framebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, camera->depthbuffer);
        glClearColor(camera->clear_color.x, camera->clear_color.y, camera->clear_color.z, camera->clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}