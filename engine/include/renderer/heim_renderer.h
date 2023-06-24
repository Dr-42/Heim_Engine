#ifndef HEIM_RENDERER_H
#define HEIM_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "math/heim_vector.h"
#include "renderer/heim_shader.h"
#include "renderer/heim_sprite.h"

typedef struct HeimRenderer {
    GLFWwindow* window;
    HeimVec2ui window_size;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    HeimShader* shader;
} HeimRenderer;

void heim_renderer_create(HeimVec2ui win_size);
void heim_renderer_close();

void heim_renderer_init(GLFWwindow* window);
void heim_renderer_update(float dt);

void heim_renderer_register_sprite(HeimSprite* sprite);
void heim_renderer_draw_sprite(HeimSprite* sprite);

#endif  // HEIM_RENDERER_H
