#ifndef HEIM_WINDOWING_H
#define HEIM_WINDOWING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "core/heim_input.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "ecs/heim_ecs.h"
#include "math/heim_vec.h"

/// @brief HeimWindow is a struct that contains all the information needed to create a window.
typedef struct HeimWindow {
    GLFWwindow *window;
    char *title;
    HeimVec2ui window_size;
    HeimVec2ui window_top_left;
    float delta_time, current_frame, last_frame;
} HeimWindow;

/// @brief Creates a new HeimWindow struct.
/// @param title The title of the window.
/// @return Pointer to the new HeimWindow struct.
void heim_window_new(char *title);

/// @brief Initializes the HeimWindow struct.
bool heim_window_init();

/// @brief Updates the HeimWindow struct.
void heim_window_update(void (*update)(float *dt), bool *running);

/// @brief Frees the HeimWindow struct.
void heim_window_close();

GLFWwindow *heim_window_get_window();

/// @brief Sets the window size.
/// @param size Size of the window.
void heim_window_set_window_size(HeimVec2ui size);

/// @brief Sets the window top left.
/// @param top_left Top left of the window.
void heim_window_set_window_top_left(HeimVec2ui top_left);

/// @brief Sets the window title.
/// @param title Title of the window.
void heim_window_set_window_title(char *title);

/// @brief Gets the window size.
/// @return Size of the window.
HeimVec2ui heim_window_get_window_size();

#endif  // HEIM_WINDOWING_H
