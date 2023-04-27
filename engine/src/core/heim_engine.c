#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/heim_engine.h"

#include "core/heim_memory.h"
#include "math/heim_vector.h"
#include "renderer/heim_renderer.h"

HeimEngine* heim_engine_new(char* title){
	HeimEngine *heim = malloc(sizeof(HeimEngine));
	heim->title = title;
	heim->window_size = (heim_vec2ui){800, 600};
	heim->window_top_left = (heim_vec2ui){0, 0};
	heim->mouse_left = false;
	heim->mouse_right = false;
	heim->mouse_pos = (heim_vec2i){0, 0};
	heim->mouse_delta = (heim_vec2i){0, 0};

	heim->delta_time = 0.0f;
	heim->last_frame = 0.0f;

	heim->logger = heim_logger_create(HEIM_LOG_LEVEL_DEBUG, stdout);
	heim->renderer = heim_renderer_new("Heim Engine", heim->logger);
	HEIM_LOG_INFO(heim->logger, "Created new Heim Engine\n");
	return heim;
	
}
void heim_engine_set_window_size(HeimEngine *heim, uint32_t x, uint32_t y){
	heim->window_size.x = x;
	heim->window_size.y = y;
	HEIM_LOG_INFO(heim->logger, "Set window size to %d, %d\n", x, y);
}
void heim_engine_set_window_top_left(HeimEngine *heim, uint32_t x, uint32_t y){
	heim->window_top_left.x = x;
	heim->window_top_left.y = y;
	HEIM_LOG_INFO(heim->logger, "Set window top left to %d, %d\n", x, y);
}

void heim_engine_init(HeimEngine *heim){
	if(!heim_renderer_init(heim->renderer)){
		HEIM_LOG_ERROR(heim->logger, "Failed to initialize renderer\n");
	}

	HEIM_LOG_INFO(heim->logger, "Initialized Heim Engine\n");
}

void heim_engine_run(HeimEngine *heim){
	heim_renderer_update(heim->renderer);
}

void heim_engine_cleanup(HeimEngine *heim){
	HEIM_LOG_INFO(heim->logger, "Cleaned up Heim Engine\n");
	heim_renderer_free(heim->renderer);
	heim_logger_free(heim->logger);
}

void heim_engine_free(HeimEngine *heim){
	free(heim);
}
