#ifndef HEIM_WINDOWING_H
#define HEIM_WINDOWING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "math/heim_vector.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "core/heim_input.h"

#include "ecs/heim_ecs.h"

/// @brief HeimWindow is a struct that contains all the information needed to create a window.
typedef struct {
	GLFWwindow *window;
	char *title;
	HeimVec2ui window_size;
	HeimVec2ui window_top_left;
	float delta_time;
	float last_frame;
	
	HeimLogger *logger;	
	HeimMemory *memory;
	HeimInput *input;
	HeimEcs *ecs;
} HeimWindow;

/// @brief Creates a new HeimWindow struct.
/// @param title The title of the window.
/// @param logger Pointer to the logger.
/// @param memory Pointer to the memory.
/// @return Pointer to the new HeimWindow struct.
HeimWindow *heim_window_new(char *title, HeimLogger *logger, HeimMemory *memory);

/// @brief Initializes the HeimWindow struct.
/// @param heim_window Pointer to the HeimWindow struct.
bool heim_window_init(HeimWindow *heim_window);

/// @brief Updates the HeimWindow struct.
/// @param heim_window Pointer to the HeimWindow struct.
void heim_window_update(HeimWindow *heim_window, void (*update)(float *dt), bool *running);

/// @brief Frees the HeimWindow struct.
/// @param heim_window Pointer to the HeimWindow struct.
void heim_window_free(HeimWindow *heim_window);

/// @brief Sets the window size.
/// @param heim_window Pointer to the HeimWindow struct.
/// @param size Size of the window.
void heim_window_set_window_size(HeimWindow *heim_window, HeimVec2ui size);

/// @brief Sets the window top left.
/// @param heim_window Pointer to the HeimWindow struct.
/// @param top_left Top left of the window.
void heim_window_set_window_top_left(HeimWindow *heim_window, HeimVec2ui top_left);

/// @brief Sets the window title.
/// @param heim_window Pointer to the HeimWindow struct.
/// @param title Title of the window.
void heim_window_set_window_title(HeimWindow *heim_window, char *title);

#endif // HEIM_WINDOWING_H
