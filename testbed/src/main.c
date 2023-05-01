#include <core/heim_engine.h>

int main(int argc, char** argv){
	HeimEngine* heim = heim_engine_new("Heim Engine");
	heim_engine_set_window_size(heim, 1000, 600);
	heim_engine_set_window_top_left(heim, 100, 100);

	heim_engine_init(heim);
	heim_engine_run(heim);
	heim_engine_cleanup(heim);
	heim_engine_free(heim);
	return 0;
}
