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

	input->mouse_pos = (heim_vec2f){0.0f, 0.0f};
	input->mouse_delta = (heim_vec2f){0.0f, 0.0f};

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
	if (input->keys[GLFW_KEY_ESCAPE]){
		HEIM_LOG_INFO(input->logger, "Input: ESC");
		glfwSetWindowShouldClose(input->window, true);
	}

	if (input->keys[GLFW_KEY_W]){
		HEIM_LOG_INFO(input->logger, "Input: W");
	}
	if (input->keys[GLFW_KEY_A]){
		HEIM_LOG_INFO(input->logger, "Input: A");
	}
	if (input->keys[GLFW_KEY_S]){
		HEIM_LOG_INFO(input->logger, "Input: S");
	}
	if (input->keys[GLFW_KEY_D]){
		HEIM_LOG_INFO(input->logger, "Input: D");
	}
	glfwPollEvents();
}

void heim_input_free(HeimInput *input){
	HEIM_FREE(input->memory, input, HEIM_MEMORY_TYPE_BASE);
}

void heim_input_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
	if (key >= 0 && key < 1024){
		if (action == GLFW_PRESS){
			keys_static[key] = true;
		} else if (action == GLFW_RELEASE){
			keys_static[key] = false;
		}
	}
}
void heim_input_mouse_button_callback(GLFWwindow *window, int button, int action, int mods){}
void heim_input_cursor_position_callback(GLFWwindow *window, double xpos, double ypos){}
void heim_input_framebuffer_size_callback(GLFWwindow *window, int width, int height){}

void heim_input_mouse_grab(HeimInput *input, bool grab){}
void heim_input_mouse_hide(HeimInput *input, bool hide){}
