#include "core/heim_input.h"
#include "core/heim_logger.h"
#include "math/heim_vector.h"

static bool keys_static[1024] = {false};

HeimInput *heim_input_create(HeimLogger *logger, HeimMemory *memory, GLFWwindow *window){
	HeimInput *input = HEIM_MALLOC(memory, HeimInput, HEIM_MEMORY_TYPE_BASE);
	input->logger = logger;
	input->memory = memory;
	input->window = window;

	input->mouse_grabbed = false;
	input->mouse_hidden = false;

	input->keys = keys_static;
	input->mouse_left = false;
	input->mouse_right = false;

	input->mouse_pos = (HeimVec2f){0.0f, 0.0f};
	input->mouse_delta = (HeimVec2f){0.0f, 0.0f};

	return input;
}

void heim_input_init(HeimInput *input){
	input->keys = keys_static;

	if (input->window != NULL){
		glfwSetKeyCallback(input->window, (GLFWkeyfun)heim_input_key_callback);
		glfwSetMouseButtonCallback(input->window, (GLFWmousebuttonfun)heim_input_mouse_button_callback);
		glfwSetCursorPosCallback(input->window, (GLFWcursorposfun)heim_input_cursor_position_callback);
		glfwSetFramebufferSizeCallback(input->window, (GLFWframebuffersizefun)heim_input_framebuffer_size_callback);
	} else {
		HEIM_LOG_ERROR(input->logger, "Input: Window is NULL");
	}
}

void heim_input_update(HeimInput *input){
	(void)input; // Fix for unused parameter warning
	glfwPollEvents();
}

void heim_input_free(HeimInput *input){
	HEIM_FREE(input->memory, input, HEIM_MEMORY_TYPE_BASE);
}

void heim_input_key_callback(GLFWwindow *window, int key, int scancode, int action, int /*mods*/){
	(void)window; // Fix for unused parameter warning
	(void)scancode; // Fix for unused parameter warning

	if (key >= 0 && key < 1024){
		if (action == GLFW_PRESS){
			keys_static[key] = true;
		} else if (action == GLFW_RELEASE){
			keys_static[key] = false;
		}
	}
}

void heim_input_mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
	(void)window; // Fix for unused parameter warning
}

void heim_input_cursor_position_callback(GLFWwindow *window, double xpos, double ypos){
	(void)window; // Fix for unused parameter warning
}

void heim_input_framebuffer_size_callback(GLFWwindow *window, int width, int height){
	(void)window; // Fix for unused parameter warning
}

void heim_input_mouse_grab(HeimInput *input, bool grab){
	(void)input; // Fix for unused parameter warning
	(void)grab; // Fix for unused parameter warning
}

void heim_input_mouse_hide(HeimInput *input, bool hide){
	(void)input; // Fix for unused parameter warning
	(void)hide; // Fix for unused parameter warning
}
