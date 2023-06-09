#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "core/heim_event.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "core/utils/heim_bitmask.h"
#include "math/heim_vec.h"

/// @brief HeimInput is a struct that contains all the information needed to handle input.
typedef struct HeimInput {
    HeimBitmask *keys;
    HeimBitmask *keys_last_frame;
    bool mouse_left, mouse_right;
    bool mouse_left_last_frame, mouse_right_last_frame;
    HeimVec2f mouse_pos;
    HeimVec2f mouse_delta;

    bool mouse_grabbed;
    bool mouse_hidden;

    HeimEventQueue *input_queue;

    GLFWwindow *window;
} HeimInput;

/// @brief Creates a new HeimInput struct.
/// @param window Pointer to the window.
/// @return Pointer to the new HeimInput struct.
void heim_input_create(GLFWwindow *window);

/// @brief Initializes the HeimInput struct.
void heim_input_init();

/// @brief Destroys the HeimInput struct.
void heim_input_destroy();

/// @brief Updates the HeimInput struct.
void heim_input_update();

void heim_input_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void heim_input_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void heim_input_cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void heim_input_framebuffer_size_callback(GLFWwindow *window, int width, int height);

void heim_input_mouse_grab(bool grab);
void heim_input_mouse_hide(bool hide);

bool heim_input_key_pressed(int key);
bool heim_input_key_released(int key);

bool heim_input_mouse_pressed(int button);
bool heim_input_mouse_released(int button);
bool heim_input_key_down(int key);
bool heim_input_key_up(int key);
bool heim_input_mouse_down(int button);
bool heim_input_mouse_up(int button);

HeimVec2f heim_input_mouse_position();
HeimVec2f heim_input_mouse_delta();