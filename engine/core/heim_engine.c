#include "core/heim_engine.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/heim_memory.h"
#include "core/heim_windowing.h"
#include "math/heim_vec.h"

HeimEngine heim = {
    .running = false,
    .fullscreen = false,
};

void heim_engine_new(char *title) {
    heim_window_new(title);
    HEIM_LOG_INFO("Created new Heim Engine");
}
void heim_engine_set_window_size(uint32_t x, uint32_t y) {
    heim_window_set_window_size((HeimVec2ui){x, y});
    HEIM_LOG_INFO("Set window size to %d, %d", x, y);
}
void heim_engine_set_window_top_left(uint32_t x, uint32_t y) {
    heim_window_set_window_top_left((HeimVec2ui){x, y});
    HEIM_LOG_INFO("Set window top left to %d, %d", x, y);
}

void heim_engine_set_fullscreen(bool fullscreen) {
    heim.fullscreen = fullscreen;
    heim_window_set_fullscreen(fullscreen);
    if (fullscreen) {
        HEIM_LOG_INFO("Set fullscreen");
    } else {
        HEIM_LOG_INFO("Disabled fullscreen");
    }
}

bool heim_engine_is_fullscreen() {
    return heim.fullscreen;
}

void heim_engine_init(void (*init)()) {
    if (!heim_window_init()) {
        HEIM_LOG_ERROR("Failed to initialize window");
    }

    heim_logger_init(NULL, HEIM_LOG_LEVEL_ALL);
    heim_ecs_create();

    heim.running = true;
    HEIM_LOG_INFO("Initialized Heim Engine");
    init();
}

void heim_engine_run(void (*update)(float dt)) {
    heim_window_update(update, &(heim.running));
}

void heim_engine_should_close(bool should_close) {
    heim.running = !should_close;
    glfwSetWindowShouldClose(heim_window_get_window(), should_close);
}

void heim_engine_cleanup() {
    HEIM_LOG_INFO("Cleaned up Heim Engine");
    heim_input_destroy();
    heim_window_close(heim_window_get_window());
    heim_memory_close();
}

void heim_engine_free() {
    // End GL
    glfwDestroyWindow(heim_window_get_window());
    glfwTerminate();
}

void heim_engine_shutdown() {
    heim_ecs_close();
    heim_logger_close();
    heim_engine_cleanup();
    heim_engine_free();
}
