#include "core/heim_input.h"

#include "core/heim_logger.h"
#include "math/heim_vector.h"

static HeimInput input = {
    .keys = {false},
    .mouse_left = false,
    .mouse_right = false,
    .mouse_pos = {0.0f, 0.0f},
    .mouse_delta = {0.0f, 0.0f},
    .mouse_grabbed = false,
    .mouse_hidden = false,
    .window = NULL};

void heim_input_create(GLFWwindow *window) {
    input.window = window;
}

void heim_input_init() {
    if (input.window != NULL) {
        glfwSetKeyCallback(input.window, (GLFWkeyfun)heim_input_key_callback);
        glfwSetMouseButtonCallback(input.window, (GLFWmousebuttonfun)heim_input_mouse_button_callback);
        glfwSetCursorPosCallback(input.window, (GLFWcursorposfun)heim_input_cursor_position_callback);
        glfwSetFramebufferSizeCallback(input.window, (GLFWframebuffersizefun)heim_input_framebuffer_size_callback);
    } else {
        HEIM_LOG_ERROR("Input: Window is NULL");
    }
}

void heim_input_update() {
    glfwPollEvents();
}

void heim_input_key_callback(GLFWwindow *window, int key, int scancode, int action, int /*mods*/) {
    (void)window;    // Fix for unused parameter warning
    (void)scancode;  // Fix for unused parameter warning

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            input.keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            input.keys[key] = false;
        }
    }
}

void heim_input_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    (void)window;  // Fix for unused parameter warning
    (void)mods;    // Fix for unused parameter warning
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            input.mouse_left = true;
        } else if (action == GLFW_RELEASE) {
            input.mouse_left = false;
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            input.mouse_right = true;
        } else if (action == GLFW_RELEASE) {
            input.mouse_right = false;
        }
    }
}

void heim_input_cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    input.mouse_delta.x = (float)xpos - input.mouse_pos.x;
    input.mouse_delta.y = (float)ypos - input.mouse_pos.y;

    input.mouse_pos.x = (float)xpos;
    input.mouse_pos.y = (float)ypos;

    if (input.mouse_grabbed) {
        glfwSetCursorPos(window, (double)input.mouse_pos.x, (double)input.mouse_pos.y);
    }
}

void heim_input_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;  // Fix for unused parameter warning
    glViewport(0, 0, width, height);
}

void heim_input_mouse_grab(bool grab) {
    input.mouse_grabbed = grab;
    glfwSetInputMode(input.window, GLFW_CURSOR, grab ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void heim_input_mouse_hide(bool hide) {
    input.mouse_hidden = hide;
    glfwSetInputMode(input.window, GLFW_CURSOR, hide ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
}

bool heim_input_key_pressed(int key) {
    return input.keys[key];
}
bool heim_input_key_released(int key) {
    return !input.keys[key];
}

bool heim_input_mouse_pressed(int button) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        return input.mouse_left;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        return input.mouse_right;
    }
    return false;
}
bool heim_input_mouse_released(int button) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        return !input.mouse_left;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        return !input.mouse_right;
    }
    return false;
}

HeimVec2f heim_input_mouse_position() {
    return input.mouse_pos;
}
HeimVec2f heim_input_mouse_delta() {
    return input.mouse_delta;
}