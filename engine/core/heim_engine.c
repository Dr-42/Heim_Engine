#include "core/heim_engine.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "core/heim_memory.h"
#include "core/heim_windowing.h"
#include "math/heim_vec.h"
#include "core/utils/trace.h"

HeimEngine heim = {
    .running = false,
    .fullscreen = false,
};

#ifdef _WIN64
int32_t NvOptimusEnablement = 0x00000001;
#endif
#ifdef __linux__
#endif


void heim_engine_handle_segfault(int sig) {
    #ifdef __linux__
    HEIM_LOG_ERROR("Heim Engine crashed with signal %s", strsignal(sig));
    #endif
    #ifdef _WIN64
    HEIM_LOG_ERROR("Heim Engine crashed with signal %d", sig);
    #endif
    print_trace();
    exit(EXIT_FAILURE);
}

void set_signal_handlers() {
    signal(SIGINT, exit);
    signal(SIGTERM, exit);
    signal(SIGABRT, exit);
    signal(SIGSEGV, heim_engine_handle_segfault);
}

void heim_engine_new(char *title) {
    #ifdef __linux__
    setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
    setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);
    #endif
    heim_logger_init(NULL, HEIM_LOG_LEVEL_ALL);
    set_signal_handlers();
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
    heim_window_close();
    heim_memory_close();
}

void heim_engine_free() {
    // End GL
    glfwDestroyWindow(heim_window_get_window());
    glfwTerminate();
}

void heim_engine_shutdown(void (*shutdown)()) {
    shutdown();
    heim_ecs_close();
    heim_logger_close();
    heim_engine_cleanup();
    heim_engine_free();
}
