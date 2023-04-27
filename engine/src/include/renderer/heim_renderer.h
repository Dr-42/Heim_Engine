#ifndef HEIM_RENDERER_H
#define HEIM_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "math/heim_vector.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"

typedef struct {
	GLFWwindow *window;
	char *title;
	heim_vec2ui window_size;
	heim_vec2ui window_top_left;
	float delta_time;
	float last_frame;
	
	HeimLogger *logger;	
	HeimMemory *memory;
} HeimRenderer;

HeimRenderer *heim_renderer_new(char *title, HeimLogger *logger, HeimMemory *memory);
bool heim_renderer_init(HeimRenderer *renderer);
void heim_renderer_update(HeimRenderer *renderer);
void heim_renderer_free(HeimRenderer *renderer);

void heim_renderer_set_window_size(HeimRenderer *renderer, heim_vec2ui size);
void heim_renderer_set_window_top_left(HeimRenderer *renderer, heim_vec2ui top_left);
void heim_renderer_set_window_title(HeimRenderer *renderer, char *title);

#endif /* HEIM_RENDERER_H */
