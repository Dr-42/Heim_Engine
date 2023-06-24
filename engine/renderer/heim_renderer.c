#include "renderer/heim_renderer.h"

#include "core/heim_logger.h"
#include "math/heim_vector.h"
#include "renderer/heim_sprite.h"

static HeimVec3f vertices[] = {
    (HeimVec3f){-1.f, -1.f, 0.0f},
    (HeimVec3f){1.f, -1.f, 0.0f},
    (HeimVec3f){1.f, 1.f, 0.0f},
    (HeimVec3f){-1.f, 1.f, 0.0f}};

static uint16_t indices[] = {0, 1, 2, 2, 3, 0};

#define HEIM_RENDERER_MAX_SPRITES 1000
static HeimSprite* sprites[HEIM_RENDERER_MAX_SPRITES];
static uint32_t sprite_count = 0;

static HeimRenderer renderer = {
    .window = NULL,
    .window_size = {0, 0},
    .vao = 0,
    .vbo = 0,
    .ebo = 0,
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glGenVertexArrays(1, &renderer.vao);
    glGenBuffers(1, &renderer.vbo);
    glGenBuffers(1, &renderer.ebo);

    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HeimVec3f), (void*)0);
    glEnableVertexAttribArray(0);
    heim_shader_bind(renderer.shader);
}

void heim_renderer_update(float dt) {
    (void)dt;
    glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    heim_shader_bind(renderer.shader);

    //glBindVertexArray(renderer.vao);
    
    for(uint32_t i = 0; i < sprite_count; i++){
        heim_renderer_draw_sprite(sprites[i]);
    }

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glfwSwapBuffers(renderer.window);
}

void heim_renderer_register_sprite(HeimSprite* sprite){
    if(sprite_count < HEIM_RENDERER_MAX_SPRITES){
        sprites[sprite_count++] = sprite;
    } else {
        HEIM_LOG_ERROR("Maximum number of sprites reached");
    }
}

void heim_renderer_draw_sprite(HeimSprite* sprite){
    GLuint vao = heim_sprite_get_vao(sprite);
    GLuint vbo = heim_sprite_get_vbo(sprite);
    GLuint ebo = heim_sprite_get_ebo(sprite);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
}
