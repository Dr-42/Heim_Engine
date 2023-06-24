#include "renderer/heim_renderer.h"

#include "math/heim_vector.h"

static HeimVec3f vertices[] = {
    (HeimVec3f){-0.5f, -0.5f, 0.0f},
    (HeimVec3f){0.5f, -0.5f, 0.0f},
    (HeimVec3f){0.0f, 0.5f, 0.0f}};

static uint32_t VAO, VBO;

static HeimRenderer renderer = {
    .window = NULL,
    .window_size = {0, 0},
    .triangle_vao = 0,
    .triangle_vbo = 0,
    .triangle_ebo = 0,
    .shader = NULL,
};

void heim_renderer_create(HeimVec2ui win_size) {
    renderer.window_size = win_size;
}

void heim_renderer_close() {
    heim_shader_free(renderer.shader);
}

void heim_renderer_init(GLFWwindow* window) {
    renderer.window = window;
    renderer.shader = heim_shader_create();
    heim_shader_init(renderer.shader, "assets/shaders/triangle_vert.glsl", "assets/shaders/triangle_frag.glsl");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    heim_shader_bind(renderer.shader);
}

void heim_renderer_update(float dt) {
    (void)dt;
    glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    heim_shader_bind(renderer.shader);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(renderer.window);
}