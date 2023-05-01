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

/// @brief Main Engine struct.
typedef struct HeimEngine{
	bool running;
	HeimLogger *logger;
	HeimMemory *memory;
	HeimWindow *heim_window;
} HeimEngine;

/// @brief Creates a new HeimEngine.
/// @param title The title of the window.
/// @return Pointer to the new HeimEngine struct.
HeimEngine* heim_engine_new(char* title);

/// @brief Sets the window size.
void heim_engine_set_window_size(HeimEngine *heim, uint32_t x, uint32_t y);
/// @brief Sets the window top left.
void heim_engine_set_window_top_left(HeimEngine *heim, uint32_t x, uint32_t y);

/// @brief Initializes the HeimEngine struct.
/// @param heim Pointer to the HeimEngine struct.
/// @param init Pointer to the user init function.
void heim_engine_init(HeimEngine *heim, void (*init)(void));

/// @brief Runs the HeimEngine struct.
/// @param heim Pointer to the HeimEngine struct.
/// @param update Pointer to the user update function.
void heim_engine_run(HeimEngine *heim, void (*update)(float *dt));

/// @brief Frees the HeimEngine struct.
/// @param heim Pointer to the HeimEngine struct.
void heim_engine_shutdown(HeimEngine *heim);

/// @brief Sets the should close flag.
/// @param heim Pointer to the HeimEngine struct.
/// @param should_close The value to set the should close flag to.
void heim_engine_should_close(HeimEngine *heim, bool should_close);

#endif /* ENGINE_H */
