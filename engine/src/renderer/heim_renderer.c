#include "renderer/heim_renderer.h"
#include <stdio.h>

float currentFrame = 0.0f;

bool heim_renderer_init(HeimRenderer *renderer){
    if (!glfwInit())
    {
    printf("Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    renderer->window = glfwCreateWindow(renderer->window_size.x, renderer->window_size.y, renderer->title, NULL, NULL);
    glfwMakeContextCurrent(renderer->window);

    // glew: load all OpenGL function pointers
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK)
    {
    printf("Failed to initialize GLEW error\n");
        glewGetErrorString(err);
        return false;
    }

    //glfwSetKeyCallback(renderer->window, Nomu::Input::keyCallback);
    //glfwSetCursorPosCallback(renderer->window, Nomu::Input::cursorPositionCallback);
    //glfwSetMouseButtonCallback(renderer->window, Nomu::Input::mouseButtonCallback);
    //glfwSetFramebufferSizeCallback(renderer->window, Nomu::Input::framebufferSizeCallback);

    // OpenGL configuration
    glViewport(renderer->window_top_left.x, renderer->window_top_left.y, renderer->window_size.x, renderer->window_size.y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void heim_renderer_update(HeimRenderer *renderer){
    while (!glfwWindowShouldClose(renderer->window))
    {
        currentFrame = glfwGetTime();
        renderer->delta_time = currentFrame - renderer->last_frame;
        glfwPollEvents();
        //mGame->ProcessInput(renderer->delta_time);
        //mGame->Update(renderer->delta_time);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //mGame->Render();
        glfwSwapBuffers(renderer->window);

        renderer->last_frame = currentFrame;
    }
}


void heim_renderer_set_window_size(HeimRenderer *renderer, heim_vec2ui size){
    renderer->window_size = size;
}
void heim_renderer_set_window_top_left(HeimRenderer *renderer, heim_vec2ui top_left){
    renderer->window_top_left = top_left;
}
void heim_renderer_set_window_title(HeimRenderer *renderer, char *title){
    renderer->title = title;
}
