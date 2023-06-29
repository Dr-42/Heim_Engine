#include <core/heim_engine.h>
#include <math/heim_math_common.h>
#include <renderer/heim_obj.h>
#include <renderer/heim_texture.h>

HeimEntity entity;
HeimComponent component;
HeimObj *object;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

float total_time = 0.0f;

HeimTexture *tex1;
HeimMat4 model_mat;
HeimMat4 view_mat;
HeimMat4 proj_mat;

void test_system(HeimEcs *ecs, float dt) {
    for (uint64_t i = 1; i < ecs->entity_count + 1; i++) {
        HeimEntity entity = ecs->entities[i];
        for (uint64_t j = 1; j < ecs->component_count + 1; j++) {
            if (ecs->components[j] == component) {
                HeimObj *obj = ecs->component_data[j][entity];
                heim_texture_bind(tex1, 0);
                heim_obj_render(obj);
            }
        }
    }
}

void testbed_init() {
    entity = heim_ecs_create_entity();
    component = heim_ecs_register_component(sizeof(HeimObj));

    object = heim_obj_load("assets/models/susan.obj");
    heim_ecs_add_component(entity, component, object);

    view_mat = heim_mat4_identity();
    view_mat = heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 3.0f}, (HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, 1.0f, 0.0f});

    HeimMat4 projection = heim_mat4_perspective(heim_math_deg_to_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    heim_shader_set_uniform_mat4(object->shader, "projection", projection);

    heim_ecs_add_system(test_system);

    tex1 = heim_create_texture("assets/textures/susan.png");
}

void testbed_update(float *dt) {
    (void)dt;
    if (heim_input_key_pressed(GLFW_KEY_ESCAPE)) {
        heim_engine_should_close(true);
    }

    heim_shader_bind(object->shader);
    if (heim_input_key_pressed(GLFW_KEY_W)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, 0.0f, 1.0f * *dt});
    }
    if (heim_input_key_pressed(GLFW_KEY_S)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, 0.0f, -1.0f * *dt});
    }
    if (heim_input_key_pressed(GLFW_KEY_A)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){1.0f * *dt, 0.0f, 0.0f});
    }
    if (heim_input_key_pressed(GLFW_KEY_D)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){-1.0f * *dt, 0.0f, 0.0f});
    }
    if (heim_input_key_pressed(GLFW_KEY_Q)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, -1.0f * *dt, 0.0f});
    }
    if (heim_input_key_pressed(GLFW_KEY_E)) {
        view_mat = heim_mat4_translate(view_mat, (HeimVec3f){0.0f, 1.0f * *dt, 0.0f});
    }

    // mouse
    HeimVec2f mouse_delta = heim_input_mouse_delta();
    view_mat = heim_mat4_rotate(view_mat, mouse_delta.x * 0.01f, (HeimVec3f){0.0f, 1.0f * *dt, 0.0f});
    view_mat = heim_mat4_rotate(view_mat, mouse_delta.y * 0.01f, (HeimVec3f){1.0f * *dt, 0.0f, 0.0f});

    if (heim_input_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
        view_mat = heim_mat4_identity();
        view_mat = heim_mat4_lookat((HeimVec3f){0.0f, 0.0f, 3.0f}, (HeimVec3f){0.0f, 0.0f, 0.0f}, (HeimVec3f){0.0f, 1.0f, 0.0f});
    }

    heim_shader_set_uniform_mat4(object->shader, "view", view_mat);
}

int main(void) {
    heim_engine_new("Heim Engine Testbed");
    heim_engine_set_window_size(WINDOW_WIDTH, WINDOW_HEIGHT);
    heim_engine_set_window_top_left(100, 100);

    heim_engine_init(testbed_init);
    heim_input_mouse_hide(true);
    heim_input_mouse_grab(true);
    heim_engine_run(testbed_update);

    heim_destroy_texture(tex1);
    heim_obj_free(object);

    heim_engine_shutdown();
    return 0;
}
