#ifndef HEIM_RENDERER_H
#define HEIM_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "math/heim_vector.h"
#include "renderer/heim_shader.h"

typedef struct HeimRenderer {
    GLFWwindow* window;
    HeimVec2ui window_size;

    GLuint triangle_vao;
    GLuint triangle_vbo;
    GLuint triangle_ebo;

    HeimShader* shader;
} HeimRenderer;

void heim_renderer_create(HeimVec2ui win_size);
void heim_renderer_close();

void heim_renderer_init(GLFWwindow* window);
void heim_renderer_update(float dt);

#endif  // HEIM_RENDERER_H