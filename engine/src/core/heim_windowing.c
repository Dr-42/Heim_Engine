#include "core/heim_windowing.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include <stdio.h>


float currentFrame = 0.0f;

HeimWindow* heim_window_new(char* title, HeimLogger *logger, HeimMemory *memory){
    HeimWindow *window = HEIM_MALLOC(memory, HeimWindow, HEIM_MEMORY_TYPE_BASE);
    window->title = title;
    window->window_size = (heim_vec2ui){800, 600};
    window->window_top_left = (heim_vec2ui){0, 0};
    window->delta_time = 0.0f;
    window->last_frame = 0.0f;
    window->logger = logger;
    window->memory = memory;
    return window;
}

bool heim_window_init(HeimWindow *window){
    if (!glfwInit())
    {
        HEIM_LOG_ERROR(window->logger, "Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    window->window = glfwCreateWindow(window->window_size.x, window->window_size.y, window->title, NULL, NULL);
    glfwMakeContextCurrent(window->window);

    // glew: load all OpenGL function pointers
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK)
    {
        HEIM_LOG_ERROR(window->logger, "Failed to initialize GLEW \n %s", glewGetErrorString(err));
        return false;
    }

    //glfwSetKeyCallback(window->window, Nomu::Input::keyCallback);
    //glfwSetCursorPosCallback(window->window, Nomu::Input::cursorPositionCallback);
    //glfwSetMouseButtonCallback(window->window, Nomu::Input::mouseButtonCallback);
    //glfwSetFramebufferSizeCallback(window->window, Nomu::Input::framebufferSizeCallback);

    // OpenGL configuration
    glViewport(window->window_top_left.x, window->window_top_left.y, window->window_size.x, window->window_size.y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void heim_window_update(HeimWindow *window){
    while (!glfwWindowShouldClose(window->window))
    {
        currentFrame = glfwGetTime();
        window->delta_time = currentFrame - window->last_frame;
        glfwPollEvents();
        //mGame->ProcessInput(window->delta_time);
        //mGame->Update(window->delta_time);
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //mGame->Render();
        glfwSwapBuffers(window->window);

        window->last_frame = currentFrame;
    }
}

void heim_window_set_window_size(HeimWindow *window, heim_vec2ui size){
    window->window_size = size;
}
void heim_window_set_window_top_left(HeimWindow *window, heim_vec2ui top_left){
    window->window_top_left = top_left;
}
void heim_window_set_window_title(HeimWindow *window, char *title){
    window->title = title;
}

void heim_window_free(HeimWindow *window){
    glfwTerminate();
    HEIM_FREE(window->memory, window, HEIM_MEMORY_TYPE_BASE);
}
