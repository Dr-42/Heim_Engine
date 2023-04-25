#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/heim_engine.h"

#include "math/heim_vector.h"
#include "renderer/heim_renderer.h"

static HeimRenderer renderer = {
				.window = NULL,
				.title = "Heim Engine",
				.window_size = (heim_vec2ui){800, 600},
				.window_top_left = (heim_vec2ui){100, 100},
				.delta_time = 0.0f,
				.last_frame = 0.0f
};

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
				return heim;
}
void heim_engine_set_window_size(HeimEngine *heim, uint32_t x, uint32_t y){
				heim->window_size.x = x;
				heim->window_size.y = y;
}
void heim_engine_set_window_top_left(HeimEngine *heim, uint32_t x, uint32_t y){
				heim->window_top_left.x = x;
				heim->window_top_left.y = y;
}

void heim_engine_init(HeimEngine *heim){
				if(!heim_renderer_init(&renderer)){
								printf("Failed to initialize renderer\n");
				}
}
void heim_engine_run(HeimEngine *heim){
				heim_renderer_update(&renderer);
}
void heim_engine_cleanup(HeimEngine *heim){}
void heim_engine_free(HeimEngine *heim){
				free(heim);
}
