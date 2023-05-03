#ifndef HEIM_RENDERER_H
#define HEIM_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math/heim_vector.h"

#include "core/heim_logger.h"
#include "core/heim_memory.h"

#include "renderer/heim_shader.h"

typedef struct HeimRenderer {
    GLFWwindow* window;
    HeimVec2ui window_size;

    HeimLogger *logger;
    HeimMemory *memory;

    GLuint triangle_vao;
    GLuint triangle_vbo;
    GLuint triangle_ebo;

    HeimShader* shader;

} HeimRenderer;

HeimRenderer* heim_renderer_create(HeimLogger* logger, HeimMemory* memory, HeimVec2ui win_size);
void heim_renderer_free(HeimRenderer* renderer, HeimMemory* memory);

void heim_renderer_init(HeimRenderer* renderer, GLFWwindow* window);
void heim_renderer_update(HeimRenderer* renderer, float dt);

#endif // HEIM_RENDERER_H