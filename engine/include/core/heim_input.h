#ifndef HEIM_INPUT_H
#define HEIM_INPUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "math/heim_vector.h"

/// @brief HeimInput is a struct that contains all the information needed to handle input.
typedef struct HeimInput {
    bool keys[1024];
    bool mouse_left, mouse_right;
    HeimVec2f mouse_pos;
    HeimVec2f mouse_delta;

    bool mouse_grabbed;
    bool mouse_hidden;

    GLFWwindow *window;
} HeimInput;

/// @brief Creates a new HeimInput struct.
/// @param window Pointer to the window.
/// @return Pointer to the new HeimInput struct.
void heim_input_create(GLFWwindow *window);

/// @brief Initializes the HeimInput struct.
void heim_input_init();

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

HeimVec2f heim_input_mouse_position();
HeimVec2f heim_input_mouse_delta();

#endif  // HEIM_INPUT_H
