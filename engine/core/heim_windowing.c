#include "core/heim_windowing.h"
#include "core/heim_input.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include <stdio.h>


float currentFrame = 0.0f;

HeimWindow* heim_window_new(char* title, HeimLogger *logger, HeimMemory *memory){
    HeimWindow *heim_window = HEIM_MALLOC(memory, HeimWindow, HEIM_MEMORY_TYPE_BASE);
    heim_window->title = title;
    heim_window->window = NULL;
    heim_window->window_size = (HeimVec2ui){800, 600};
    heim_window->window_top_left = (HeimVec2ui){0, 0};
    heim_window->delta_time = 0.0f;
    heim_window->last_frame = 0.0f;
    heim_window->logger = logger;
    heim_window->memory = memory;
    heim_window->input = heim_input_create(logger, memory, heim_window->window);
    heim_window->ecs = heim_ecs_create(logger, memory);
    return heim_window;
}

bool heim_window_init(HeimWindow *heim_window){
    if (!glfwInit())
    {
        HEIM_LOG_ERROR(heim_window->logger, "Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    heim_window->window = glfwCreateWindow(heim_window->window_size.x, heim_window->window_size.y, heim_window->title, NULL, NULL);
    
    if (heim_window->window == NULL)
    {
        HEIM_LOG_ERROR(heim_window->logger, "Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(heim_window->window);

    // glew: load all OpenGL function pointers
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK)
    {
        HEIM_LOG_ERROR(heim_window->logger, "Failed to initialize GLEW \n %s", glewGetErrorString(err));
        return false;
    }


    heim_window->input->window = heim_window->window;
    heim_input_init(heim_window->input);
    // OpenGL configuration
    glViewport(heim_window->window_top_left.x, heim_window->window_top_left.y, heim_window->window_size.x, heim_window->window_size.y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void heim_window_update(HeimWindow *heim_window, void (*update)(float *dt), bool* running){
    while (!glfwWindowShouldClose(heim_window->window) && *running)
    {
        currentFrame = glfwGetTime();
        heim_window->delta_time = currentFrame - heim_window->last_frame;
        glfwPollEvents();
        heim_input_update(heim_window->input);

        update(&heim_window->delta_time);

        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        heim_ecs_update(heim_window->ecs, heim_window->delta_time);

        glfwSwapBuffers(heim_window->window);

        heim_window->last_frame = currentFrame;
    }
}

void heim_window_set_window_size(HeimWindow *heim_window, HeimVec2ui size){
    heim_window->window_size = size;
}
void heim_window_set_window_top_left(HeimWindow *heim_window, HeimVec2ui top_left){
    heim_window->window_top_left = top_left;
}
void heim_window_set_window_title(HeimWindow *heim_window, char *title){
    heim_window->title = title;
}

void heim_window_free(HeimWindow *heim_window){
    glfwTerminate();
    heim_input_free(heim_window->input);
    heim_ecs_free(heim_window->ecs, heim_window->memory);
    HEIM_FREE(heim_window->memory, heim_window, HEIM_MEMORY_TYPE_BASE);
}
