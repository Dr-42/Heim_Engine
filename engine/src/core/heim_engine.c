#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/heim_engine.h"

#include "core/heim_memory.h"
#include "math/heim_vector.h"
#include "core/heim_windowing.h"

HeimEngine* heim_engine_new(char* title){
	HeimEngine *heim = malloc(sizeof(HeimEngine));
	heim->logger = heim_logger_create(HEIM_LOG_LEVEL_DEBUG, stdout);
	heim->memory = heim_memory_create(heim->logger);
	heim->heim_window = heim_window_new("Heim Engine", heim->logger, heim->memory);

	HEIM_LOG_INFO(heim->logger, "Created new Heim Engine");
	return heim;
	
}
void heim_engine_set_window_size(HeimEngine *heim, uint32_t x, uint32_t y){
	heim->heim_window->window_size.x = x;
	heim->heim_window->window_size.y = y;
	HEIM_LOG_INFO(heim->logger, "Set window size to %d, %d", x, y);
}
void heim_engine_set_window_top_left(HeimEngine *heim, uint32_t x, uint32_t y){
	heim->heim_window->window_top_left.x = x;
	heim->heim_window->window_top_left.y = y;
	HEIM_LOG_INFO(heim->logger, "Set window top left to %d, %d", x, y);
}

void heim_engine_init(HeimEngine *heim){
	if(!heim_window_init(heim->heim_window)){
		HEIM_LOG_ERROR(heim->logger, "Failed to initialize window");
	}

	HEIM_LOG_INFO(heim->logger, "Initialized Heim Engine");
}

void heim_engine_run(HeimEngine *heim){
	heim_window_update(heim->heim_window);
}

void heim_engine_cleanup(HeimEngine *heim){
	HEIM_LOG_INFO(heim->logger, "Cleaned up Heim Engine");
	heim_window_free(heim->heim_window);
	heim_memory_free(heim->memory);
	heim_logger_free(heim->logger);
}

void heim_engine_free(HeimEngine *heim){
	free(heim);
}
