#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "core/heim_windowing.h"
#include "math/heim_vec.h"

/// @brief Main Engine struct.
typedef struct HeimEngine {
    bool running;
} HeimEngine;

/// @brief Creates a new HeimEngine.
/// @param title The title of the window.
/// @return Pointer to the new HeimEngine struct.
void heim_engine_new(char *title);

/// @brief Sets the window size.
void heim_engine_set_window_size(uint32_t x, uint32_t y);
/// @brief Sets the window top left.
void heim_engine_set_window_top_left(uint32_t x, uint32_t y);

/// @brief Initializes the HeimEngine struct.
/// @param heim Pointer to the HeimEngine struct.
/// @param init Pointer to the user init function.
void heim_engine_init(void (*init)(void));

/// @brief Runs the HeimEngine struct.
/// @param heim Pointer to the HeimEngine struct.
/// @param update Pointer to the user update function.
void heim_engine_run(void (*update)(float *dt));

/// @brief Frees the HeimEngine struct.
/// @param heim Pointer to the HeimEngine struct.
void heim_engine_shutdown();

/// @brief Sets the should close flag.
/// @param heim Pointer to the HeimEngine struct.
/// @param should_close The value to set the should close flag to.
void heim_engine_should_close(bool should_close);

#endif /* ENGINE_H */
