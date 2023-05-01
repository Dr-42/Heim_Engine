#include <core/heim_engine.h>

HeimEngine* heim;

HeimEntity entity;
HeimComponent component;

typedef struct {
	float x;
	float y;
} Position;

Position position = {0.0f, 0.0f};

HeimSystem test_system(HeimEcs *ecs, float dt){
	position.x += 0.1f;
	position.y += 0.1f;
	//HEIM_LOG_INFO(heim->logger, "Position: %f, %f", position.x, position.y);
}

void testbed_init(){
	HEIM_LOG_INFO(heim->logger, "Testbed init");
	entity = heim_ecs_create_entity(heim->heim_window->ecs);
	component = heim_ecs_add_component(heim->heim_window->ecs, entity, &position, sizeof(Position));

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

