#include "renderer/heim_renderer.h"

#include "core/heim_logger.h"
#include "math/heim_mat.h"
// #include "renderer/heim_sprite.h"
#include "core/heim_input.h"
#include "renderer/heim_obj.h"
#include "renderer/heim_shader.h"
#include "renderer/heim_texture.h"

HeimObj* O;
HeimMat4 model_mat;
HeimMat4 view_mat;
HeimMat4 proj_mat;
HeimTexture* tex;

#define HEIM_RENDERER_MAX_SPRITES 1000
static HeimSprite* sprites[HEIM_RENDERER_MAX_SPRITES];
static uint32_t sprite_count = 0;

static HeimRenderer renderer = {
    .window = NULL,
    .window_size = {0, 0},
    .shader = NULL,
};

void heim_renderer_create(HeimVec2ui win_size) {
    renderer.window_size = win_size;
}

void heim_renderer_close() {
    heim_shader_free(renderer.shader);
    heim_destroy_texture(tex);

    for (uint32_t i = 0; i < sprite_count; ++i) {
        heim_sprite_free(sprites[i]);
    }

    heim_obj_free(O);
}

void heim_renderer_init(GLFWwindow* window) {
    renderer.window = window;
    renderer.shader = heim_shader_create();
    heim_shader_init(renderer.shader, "assets/shaders/model.vert", "assets/shaders/model.frag");

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    heim_shader_bind(renderer.shader);

    model_mat = heim_mat4_identity();
    model_mat = heim_mat4_translate(model_mat, (HeimVec3f){0.0f, 0.0f, 0.0f});

    view_mat = heim_mat4_identity();
    view_mat = heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 3.0f}, (HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, 1.0f, 0.0f});

    proj_mat = heim_mat4_identity();
    proj_mat = heim_mat4_perspective(45.0f, (float)renderer.window_size.x / (float)renderer.window_size.y, 0.1f, 100.0f);

    heim_shader_set_uniform_mat4(renderer.shader, "model", model_mat);
    heim_shader_set_uniform_mat4(renderer.shader, "view", view_mat);
    heim_shader_set_uniform_mat4(renderer.shader, "projection", proj_mat);

    O = heim_obj_load("assets/models/backpack.obj");
    tex = heim_create_texture("assets/textures/backpack.jpg");
}

void heim_renderer_update(float dt) {
    (void)dt;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    heim_shader_bind(renderer.shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    /*
    for (uint32_t i = 0; i < sprite_count; i++) {
        heim_renderer_draw_sprite(sprites[i]);
    }
    */

    heim_obj_render(O);

    // Control camera using the view matrix
    if (heim_input_key_pressed(GLFW_KEY_W)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, 0.0f, 1.0f * dt});
    }
    if (heim_input_key_pressed(GLFW_KEY_S)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, 0.0f, -1.0f * dt});
    }
    if (heim_input_key_pressed(GLFW_KEY_A)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){1.0f * dt, 0.0f, 0.0f});
    }
    if (heim_input_key_pressed(GLFW_KEY_D)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){-1.0f * dt, 0.0f, 0.0f});
    }
    if (heim_input_key_pressed(GLFW_KEY_Q)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, -1.0f * dt, 0.0f});
    }
    if (heim_input_key_pressed(GLFW_KEY_E)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, 1.0f * dt, 0.0f});
    }

    // mouse
    HeimVec2f mouse_delta = heim_input_mouse_delta();
    view_mat = heim_mat4_rotate(view_mat, mouse_delta.x * 0.01f, (HeimVec3f){0.0f, 1.0f * dt, 0.0f});
    view_mat = heim_mat4_rotate(view_mat, mouse_delta.y * 0.01f, (HeimVec3f){1.0f * dt, 0.0f, 0.0f});

    if (heim_input_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
        view_mat = heim_mat4_identity();
        view_mat = heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 3.0f}, (HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, 1.0f, 0.0f});
    }

    heim_shader_set_uniform_mat4(renderer.shader, "view", view_mat);

    glfwSwapBuffers(renderer.window);
}

/*

void heim_renderer_register_sprite(HeimSprite* sprite) {
    heim_sprite_set_projection(sprite, heim_mat4_ortho(0.0f, renderer.window_size.x, renderer.window_size.y, 0.0f, -1.0f, 1.0f));
    if (sprite_count < HEIM_RENDERER_MAX_SPRITES) {
        sprites[sprite_count++] = sprite;
    } else {
        HEIM_LOG_ERROR("Maximum number of sprites reached");
    }
}

void heim_renderer_draw_sprite(HeimSprite* sprite) {
    GLuint vao = heim_sprite_get_vao(sprite);
    GLuint vbo = heim_sprite_get_vbo(sprite);
    GLuint ebo = heim_sprite_get_ebo(sprite);

    heim_shader_bind(renderer.shader);

    heim_shader_set_uniform_mat4(renderer.shader, "model", heim_sprite_get_model(sprite));
    heim_shader_set_uniform_mat4(renderer.shader, "view", heim_sprite_get_view(sprite));
    heim_shader_set_uniform_mat4(renderer.shader, "projection", heim_sprite_get_projection(sprite));

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heim_sprite_get_texture(sprite)->id);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
}

*/