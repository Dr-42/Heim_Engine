#include "renderer/heim_renderer.h"
#include "math/heim_vector.h"

static HeimVec3f vertices[] = {
    (HeimVec3f){-0.5f, -0.5f, 0.0f},
    (HeimVec3f){0.5f, -0.5f, 0.0f},
    (HeimVec3f){0.0f, 0.5f, 0.0f}
};

static uint32_t VAO, VBO;

HeimRenderer* heim_renderer_create(HeimLogger* logger, HeimMemory* memory, HeimVec2ui win_size){
    HeimRenderer* renderer = HEIM_MALLOC(memory, HeimRenderer, HEIM_MEMORY_TYPE_RENDERER);
    renderer->logger = logger;
    renderer->memory = memory;

    renderer->triangle_vao = 0;
    renderer->triangle_vbo = 0;
    renderer->triangle_ebo = 0;

    renderer->shader = heim_shader_create(renderer->logger, renderer->memory);
    renderer->camera = heim_camera_create(logger, memory, 
        (HeimVec3f){0.0f, 0.0f, -10.0f},
        (HeimVec3f){0.0f, 1.0f, 0.0f},
        -90.0f,
        0.0f);

    renderer->window_size = win_size;
    renderer->window = NULL;
    return renderer;
}

void heim_renderer_free(HeimRenderer* renderer, HeimMemory* memory){
    heim_shader_free(renderer->shader);
    heim_camera_free(renderer->camera);
    HEIM_FREE(memory, renderer, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_renderer_init(HeimRenderer *renderer, GLFWwindow *window){
    renderer->window = window;
    heim_shader_init(renderer->shader, "assets/shaders/triangle_vert.glsl", "assets/shaders/triangle_frag.glsl");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    heim_shader_bind(renderer->shader);
}

void heim_renderer_update(HeimRenderer *renderer, float dt)
{
    glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    heim_shader_bind(renderer->shader);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(renderer->window);
}