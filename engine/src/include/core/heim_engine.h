#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/heim_memory.h"
#include "math/heim_vector.h"
#include "core/heim_windowing.h"
#include "core/heim_logger.h"

typedef struct HeimEngine{
	bool running;
	HeimLogger *logger;
	HeimMemory *memory;
	HeimWindow *heim_window;
} HeimEngine;

HeimEngine* heim_engine_new(char* title);
void heim_engine_set_window_size(HeimEngine *heim, uint32_t x, uint32_t y);
void heim_engine_set_window_top_left(HeimEngine *heim, uint32_t x, uint32_t y);

void heim_engine_init(HeimEngine *heim);
void heim_engine_run(HeimEngine *heim, void (*update)(float *dt));
void heim_engine_shutdown(HeimEngine *heim);

void heim_engine_should_close(HeimEngine *heim, bool should_close);

#endif /* ENGINE_H */
