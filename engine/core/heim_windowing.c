#include "core/heim_windowing.h"

#include <stdio.h>

#include "core/heim_input.h"
#include "core/heim_logger.h"
#include "core/heim_memory.h"

static HeimWindow heim_window = {0};

void heim_window_new(char *title) {
    heim_window.title = title;
    heim_window.window = NULL;
    heim_window.window_size = (HeimVec2ui){800, 600};
    heim_window.window_top_left = (HeimVec2ui){0, 0};
    heim_window.delta_time = 0.0f;
    heim_window.last_frame = 0.0f;
}

bool heim_window_init() {
    if (!glfwInit()) {
        HEIM_LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    heim_window.window = glfwCreateWindow(heim_window.window_size.x, heim_window.window_size.y, heim_window.title, NULL, NULL);

    if (heim_window.window == NULL) {
        HEIM_LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(heim_window.window);

    // glew: load all OpenGL function pointers
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK) {
        HEIM_LOG_ERROR("Failed to initialize GLEW \n %s", glewGetErrorString(err));
        return false;
    }

    glViewport(0, 0, heim_window.window_size.x, heim_window.window_size.y);

    heim_input_create(heim_window.window);
    heim_input_init();

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

    return true;
}

void heim_window_update(void (*update)(float *dt), bool *running) {
    while (*running) {
        if (glfwWindowShouldClose(heim_window.window)) {
            *running = false;
        }

        heim_window.current_frame = glfwGetTime();
        heim_window.delta_time = heim_window.current_frame - heim_window.last_frame;
        heim_window.last_frame = heim_window.current_frame;
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        heim_input_update();
        update(&heim_window.delta_time);

        heim_ecs_update(heim_window.delta_time);
        glfwSwapBuffers(heim_window.window);

        glfwPollEvents();
    }
}

GLFWwindow *heim_window_get_window() {
    return heim_window.window;
}

void heim_window_set_window_size(HeimVec2ui size) {
    heim_window.window_size = size;
}
void heim_window_set_window_top_left(HeimVec2ui top_left) {
    heim_window.window_top_left = top_left;
}
void heim_window_set_window_title(char *title) {
    heim_window.title = title;
}

HeimVec2ui heim_window_get_window_size() {
    return heim_window.window_size;
}

void heim_window_close() {
    glfwTerminate();
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
