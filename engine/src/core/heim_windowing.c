#include "core/heim_windowing.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include <stdio.h>


float currentFrame = 0.0f;

HeimWindow* heim_window_new(char* title, HeimLogger *logger, HeimMemory *memory){
    HeimWindow *heim_window = HEIM_MALLOC(memory, HeimWindow, HEIM_MEMORY_TYPE_BASE);
    heim_window->title = title;
    heim_window->window_size = (heim_vec2ui){800, 600};
    heim_window->window_top_left = (heim_vec2ui){0, 0};
    heim_window->delta_time = 0.0f;
    heim_window->last_frame = 0.0f;
    heim_window->logger = logger;
    heim_window->memory = memory;
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
    glfwMakeContextCurrent(heim_window->window);

    // glew: load all OpenGL function pointers
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK)
    {
        HEIM_LOG_ERROR(heim_window->logger, "Failed to initialize GLEW \n %s", glewGetErrorString(err));
        return false;
    }

    //glfwSetKeyCallback(window->window, Nomu::Input::keyCallback);
    //glfwSetCursorPosCallback(window->window, Nomu::Input::cursorPositionCallback);
    //glfwSetMouseButtonCallback(window->window, Nomu::Input::mouseButtonCallback);
    //glfwSetFramebufferSizeCallback(window->window, Nomu::Input::framebufferSizeCallback);

    // OpenGL configuration
    glViewport(heim_window->window_top_left.x, heim_window->window_top_left.y, heim_window->window_size.x, heim_window->window_size.y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void heim_window_update(HeimWindow *heim_window){
    while (!glfwWindowShouldClose(heim_window->window))
    {
        currentFrame = glfwGetTime();
        heim_window->delta_time = currentFrame - heim_window->last_frame;
        glfwPollEvents();
        //mGame->ProcessInput(window->delta_time);
        //mGame->Update(window->delta_time);
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //mGame->Render();
        glfwSwapBuffers(heim_window->window);

        heim_window->last_frame = currentFrame;
    }
}

void heim_window_set_window_size(HeimWindow *heim_window, heim_vec2ui size){
    heim_window->window_size = size;
}
void heim_window_set_window_top_left(HeimWindow *heim_window, heim_vec2ui top_left){
    heim_window->window_top_left = top_left;
}
void heim_window_set_window_title(HeimWindow *heim_window, char *title){
    heim_window->title = title;
}

void heim_window_free(HeimWindow *heim_window){
    glfwTerminate();
    HEIM_FREE(heim_window->memory, heim_window, HEIM_MEMORY_TYPE_BASE);
}
