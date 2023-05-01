#ifndef HEIM_WINDOWING_H
#define HEIM_WINDOWING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "math/heim_vector.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "core/heim_input.h"

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
} HeimWindow;

HeimWindow *heim_window_new(char *title, HeimLogger *logger, HeimMemory *memory);
bool heim_window_init(HeimWindow *heim_window);
void heim_window_update(HeimWindow *heim_window, void (*update)(float *dt), bool *running);
void heim_window_free(HeimWindow *heim_window);

void heim_window_set_window_size(HeimWindow *heim_window, HeimVec2ui size);
void heim_window_set_window_top_left(HeimWindow *heim_window, HeimVec2ui top_left);
void heim_window_set_window_title(HeimWindow *heim_window, char *title);

#endif // HEIM_WINDOWING_H
