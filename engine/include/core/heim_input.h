#ifndef HEIM_INPUT_H
#define HEIM_INPUT_H

#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "math/heim_vector.h"

typedef struct {
	bool *keys;
	bool mouse_left, mouse_right;
	HeimVec2f mouse_pos;
	HeimVec2f mouse_delta;

	bool mouse_grabbed;
	bool mouse_hidden;

	HeimLogger *logger;
	HeimMemory *memory;
	GLFWwindow *window;
} HeimInput;

HeimInput *heim_input_create(HeimLogger *logger, HeimMemory *memory, GLFWwindow *window);
void heim_input_init(HeimInput *input);
void heim_input_update(HeimInput *input);
void heim_input_free(HeimInput *input);

void heim_input_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void heim_input_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void heim_input_cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void heim_input_framebuffer_size_callback(GLFWwindow *window, int width, int height);

void heim_input_mouse_grab(HeimInput *input, bool grab);
void heim_input_mouse_hide(HeimInput *input, bool hide);

#endif // HEIM_INPUT_H
