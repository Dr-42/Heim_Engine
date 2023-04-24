#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "math/heim_vector.h"

typedef struct HeimEngine{
	GLFWwindow *window;
	char* title;
	heim_vec2ui window_size;
	heim_vec2ui window_top_left;
	bool keys[1024];
	bool mouse_left, mouse_right;
	heim_vec2i mouse_pos;
	heim_vec2i mouse_delta;

	float delta_time;
	float last_frame;
} HeimEngine;

HeimEngine* heim_engine_new(char* title);
void heim_engine_set_window_size(uint32_t x, uint32_t y);
void heim_engine_set_window_top_left(uint32_t x, uint32_t y);

void heim_engine_init(HeimEngine *heim);
void heim_engine_run(HeimEngine *heim);
void heim_engine_cleanup(HeimEngine *heim);
void heim_engine_free(HeimEngine *heim);

#endif /* ENGINE_H */
