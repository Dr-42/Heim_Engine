#include "core/heim_windowing.h"

#include <stdio.h>

#include "core/heim_input.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"

static HeimWindow heim_window = {0};

void heim_window_new(char *title) {
    heim_window.title = title;
    heim_window.window = NULL;
    heim_window.window_size = (HeimVec2ui){800, 600};
    heim_window.window_top_left = (HeimVec2ui){0, 0};
    heim_window.delta_time = 0.0f;
    heim_window.last_frame = 0.0f;
}

bool heim_window_init() {
    if (!glfwInit()) {
        HEIM_LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    heim_window.window = glfwCreateWindow(heim_window.window_size.x, heim_window.window_size.y, heim_window.title, NULL, NULL);

    if (heim_window.window == NULL) {
        HEIM_LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(heim_window.window);

    // glew: load all OpenGL function pointers
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK) {
        HEIM_LOG_ERROR("Failed to initialize GLEW \n %s", glewGetErrorString(err));
        return false;
    }

    glViewport(0, 0, heim_window.window_size.x, heim_window.window_size.y);

    heim_renderer_init(heim_window.window);
    heim_input_create(heim_window.window);
    heim_input_init();

    return true;
}

void heim_window_update(void (*update)(float *dt), bool *running) {
    while (*running) {
        if (glfwWindowShouldClose(heim_window.window)) {
            *running = false;
        }

        heim_window.current_frame = glfwGetTime();
        heim_window.delta_time = heim_window.current_frame - heim_window.last_frame;
        heim_window.last_frame = heim_window.current_frame;

        heim_input_update();

        heim_renderer_update(heim_window.delta_time);

        update(&heim_window.delta_time);

        heim_ecs_update(heim_window.delta_time);

        glfwPollEvents();
    }
}

GLFWwindow *heim_window_get_window() {
    return heim_window.window;
}

void heim_window_set_window_size(HeimVec2ui size) {
    heim_window.window_size = size;
}
void heim_window_set_window_top_left(HeimVec2ui top_left) {
    heim_window.window_top_left = top_left;
}
void heim_window_set_window_title(char *title) {
    heim_window.title = title;
}

void heim_window_close() {
    glfwTerminate();
    heim_renderer_close();
}
