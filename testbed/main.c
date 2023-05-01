#include <core/heim_engine.h>

HeimEngine* heim;

HeimEntity entity, entity2;
HeimComponent component;

typedef struct {
	float x;
	float y;
} Position;

Position position = {0.0f, 0.0f};
Position position2 = {10.0f, 10.0f};

HeimSystem test_system(HeimEcs *ecs, float dt){
	for (int i = 1; i < ecs->entity_count + 1; i++){
		HeimEntity entity = ecs->entities[i];
		for (int j = 1; j < ecs->component_count + 1; j++){
			if(ecs->components[j] == component){
				Position *pos = ecs->component_data[j][entity];
				pos->x += 0.1f;
				pos->y += 0.1f;
				HEIM_LOG_INFO(heim->logger, "Position: %f, %f", pos->x, pos->y);
			}
		}
	}
}

void testbed_init(){
	HEIM_LOG_INFO(heim->logger, "Testbed init");
	entity = heim_ecs_create_entity(heim->heim_window->ecs);
	component = heim_ecs_register_component(heim->heim_window->ecs, sizeof(Position));
	heim_ecs_add_component(heim->heim_window->ecs, entity, component, &position);

	entity2 = heim_ecs_create_entity(heim->heim_window->ecs);
	heim_ecs_add_component(heim->heim_window->ecs, entity2, component, &position2);

	heim_ecs_add_system(heim->heim_window->ecs, test_system);
}

void testbed_update(float *dt){
	if(heim == NULL){
		return;
	}
	if(heim->heim_window->input->keys[GLFW_KEY_ESCAPE]){
		heim_engine_should_close(heim, true);
	}
}

int main(int argc, char** argv){
	heim = heim_engine_new("Heim Engine");
	heim_engine_set_window_size(heim, 1000, 600);
	heim_engine_set_window_top_left(heim, 100, 100);

	heim_engine_init(heim, testbed_init);
	heim_engine_run(heim, testbed_update);
	heim_engine_shutdown(heim);
	return 0;
}