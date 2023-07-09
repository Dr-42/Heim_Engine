#include "core/heim_input.h"

#include "core/heim_logger.h"
#include "math/heim_vec.h"

#define MAX_KEYS 1024

static HeimInput input = {0};

void heim_input_create(GLFWwindow *window) {
    input.window = window;
    input.input_queue = heim_event_queue_create();
    input.keys = heim_bitmask_create(MAX_KEYS);
    input.keys_last_frame = heim_bitmask_create(MAX_KEYS);
}

void heim_input_destroy() {
    heim_event_queue_destroy(input.input_queue);
    heim_bitmask_destroy(input.keys);
    heim_bitmask_destroy(input.keys_last_frame);
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
    input.mouse_delta = (HeimVec2f){0.0f, 0.0f};
    heim_bitmask_copy(input.keys_last_frame, input.keys);
    input.mouse_left_last_frame = input.mouse_left;
    input.mouse_right_last_frame = input.mouse_right;
    while (!heim_event_queue_is_empty(input.input_queue)) {
        HeimEvent event = heim_event_pop(input.input_queue);
        switch (event.type) {
            case HEIM_EVENT_TYPE_KEY_DOWN:
                heim_bitmask_set(input.keys, event.data.i[0]);
                break;

            case HEIM_EVENT_TYPE_KEY_UP:
                heim_bitmask_unset(input.keys, event.data.i[0]);
                break;

            case HEIM_EVENT_TYPE_MOUSE_DOWN:
                if (event.data.i[0] == GLFW_MOUSE_BUTTON_LEFT) {
                    input.mouse_left = true;
                } else if (event.data.i[0] == GLFW_MOUSE_BUTTON_RIGHT) {
                    input.mouse_right = true;
                }
                break;

            case HEIM_EVENT_TYPE_MOUSE_UP:
                if (event.data.i[0] == GLFW_MOUSE_BUTTON_LEFT) {
                    input.mouse_left = false;
                } else if (event.data.i[0] == GLFW_MOUSE_BUTTON_RIGHT) {
                    input.mouse_right = false;
                }
                break;

            case HEIM_EVENT_TYPE_MOUSE_MOVE:
                input.mouse_pos = event.data.vec2f[0];
                input.mouse_delta = heim_vec2f_sub(event.data.vec2f[0], event.data.vec2f[1]);
                break;
            default:
                break;
        }
    }
}

void heim_input_key_callback(GLFWwindow *window, int key, int scancode, int action, int /*mods*/) {
    (void)window;    // Fix for unused parameter warning
    (void)scancode;  // Fix for unused parameter warning

    if (key >= 0 && key < MAX_KEYS) {
        HeimEvent event;
        if (action == GLFW_PRESS) {
            event = (HeimEvent){
                .type = HEIM_EVENT_TYPE_KEY_DOWN,
                .data = (HeimEventData){
                    .i = {key},
                }};
        } else if (action == GLFW_RELEASE) {
            event = (HeimEvent){
                .type = HEIM_EVENT_TYPE_KEY_UP,
                .data = (HeimEventData){
                    .i = {key},
                }};
        }

        heim_event_push(input.input_queue, event);
    }
}

void heim_input_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    (void)window;  // Fix for unused parameter warning
    (void)mods;    // Fix for unused parameter warning
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        HeimEvent event;
        if (action == GLFW_PRESS) {
            event = (HeimEvent){
                .type = HEIM_EVENT_TYPE_MOUSE_DOWN,
                .data = (HeimEventData){
                    .i = {button},
                }};
        } else if (action == GLFW_RELEASE) {
            event = (HeimEvent){
                .type = HEIM_EVENT_TYPE_MOUSE_UP,
                .data = (HeimEventData){
                    .i = {button},
                }};
        }
        heim_event_push(input.input_queue, event);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        HeimEvent event;
        if (action == GLFW_PRESS) {
            event = (HeimEvent){
                .type = HEIM_EVENT_TYPE_MOUSE_DOWN,
                .data = (HeimEventData){
                    .i = {button},
                }};
        } else if (action == GLFW_RELEASE) {
            event = (HeimEvent){
                .type = HEIM_EVENT_TYPE_MOUSE_UP,
                .data = (HeimEventData){
                    .i = {button},
                }};
        }
        heim_event_push(input.input_queue, event);
    }
}

HeimVec2f mouse_last_pos = {0.0f, 0.0f};
void heim_input_cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    (void)window;  // Fix for unused parameter warning
    HeimEvent event = (HeimEvent){
        .type = HEIM_EVENT_TYPE_MOUSE_MOVE,
        .data = (HeimEventData){
            .vec2f = {(HeimVec2f){xpos, ypos}, mouse_last_pos},
        }};
    heim_event_push(input.input_queue, event);
    mouse_last_pos = (HeimVec2f){xpos, ypos};

    if (input.mouse_grabbed) {
        glfwSetCursorPos(window, xpos, ypos);
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
    return heim_bitmask_get(input.keys, key);
}
bool heim_input_key_released(int key) {
    return !heim_bitmask_get(input.keys, key);
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

bool heim_input_key_down(int key) {
    return heim_bitmask_get(input.keys, key) && !heim_bitmask_get(input.keys_last_frame, key);
}

bool heim_input_key_up(int key) {
    return !heim_bitmask_get(input.keys, key) && heim_bitmask_get(input.keys_last_frame, key);
}

bool heim_input_mouse_down(int button) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return input.mouse_left && !input.mouse_left_last_frame;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return input.mouse_right && !input.mouse_right_last_frame;
    }
    return false;
}

bool heim_input_mouse_up(int button) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return !input.mouse_left && input.mouse_left_last_frame;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return !input.mouse_right && input.mouse_right_last_frame;
    }
    return false;
}

HeimVec2f heim_input_mouse_position() {
    return input.mouse_pos;
}
HeimVec2f heim_input_mouse_delta() {
    HeimVec2f delta = input.mouse_delta;
    return delta;
}