#include <core/heim_engine.h>

HeimEngine* heim;

void testbed_init(){
	HEIM_LOG_INFO(heim->logger, "Testbed init");
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

