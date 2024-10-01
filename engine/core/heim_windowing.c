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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_SAMPLES, 4);
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

    heim_input_create(heim_window.window);
    heim_input_init();

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void heim_window_update(void (*update)(float dt), bool *running) {
    while (*running) {
        if (glfwWindowShouldClose(heim_window.window)) {
            *running = false;
        }

        heim_window.current_frame = glfwGetTime();
        heim_window.delta_time = heim_window.current_frame - heim_window.last_frame;
        heim_window.last_frame = heim_window.current_frame;
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        heim_input_update();
        update(heim_window.delta_time);

        heim_ecs_update(heim_window.delta_time);
        glfwSwapBuffers(heim_window.window);

        glfwPollEvents();
    }
}

GLFWwindow *heim_window_get_window() { return heim_window.window; }

void heim_window_set_window_size(HeimVec2ui size) {
    heim_window.window_size = size;
    if (heim_window.window) glfwSetWindowSize(heim_window.window, size.x, size.y);
}
void heim_window_set_window_top_left(HeimVec2ui top_left) {
    heim_window.window_top_left = top_left;
    if (heim_window.window) glfwSetWindowPos(heim_window.window, top_left.x, top_left.y);
}
void heim_window_set_window_title(char *title) { heim_window.title = title; }

HeimVec2ui heim_window_get_window_size() { return heim_window.window_size; }

void heim_window_set_fullscreen(bool fullscreen) {
    if (fullscreen) {
        glfwSetWindowMonitor(heim_window.window, glfwGetPrimaryMonitor(), 0, 0, heim_window.window_size.x,
                             heim_window.window_size.y, GLFW_DONT_CARE);
    } else {
        glfwSetWindowMonitor(heim_window.window, NULL, heim_window.window_top_left.x, heim_window.window_top_left.y,
                             heim_window.window_size.x, heim_window.window_size.y, GLFW_DONT_CARE);
    }
}

void heim_window_close() { glfwTerminate(); }
