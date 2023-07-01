#include "ecs/predef_systems/heim_model_renderer.h"

#include "core/heim_input.h"
#include "ecs/heim_ecs_predef.h"
#include "math/heim_mat.h"
#include "math/heim_math_common.h"
void heim_model_renderer_system(HeimEcs* ecs, HeimEntity entity, float dt) {
    (void)dt;
    bool has_model = heim_ecs_has_component(ecs, entity, get_model_component());
    bool has_transform = heim_ecs_has_component(ecs, entity, get_transform_component());

    if (!has_model || !has_transform) {
        return;
    }

    if (has_model && !has_transform) {
        HEIM_LOG_WARN("Entity has model component but not transform component");
        return;
    }

    HeimModel* model = heim_ecs_get_component_data(ecs, entity, get_model_component());
    HeimTransform* transform = heim_ecs_get_component_data(ecs, entity, get_transform_component());

    heim_model_render(model, transform->position, transform->rotation, transform->size, &dt);
}
HeimMat4 view_mat = (HeimMat4){0};

void heim_model_render(HeimModel* model, HeimVec3f position, HeimVec3f rotation, HeimVec3f scale, float* dt) {
    HeimMat4 model_matrix = heim_mat4_identity();
    model_matrix = heim_mat4_translate(model_matrix, position);
    model_matrix = heim_mat4_rotate(model_matrix, rotation.x, (HeimVec3f){1.0f, 0.0f, 0.0f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.y, (HeimVec3f){0.0f, 1.0f, 0.0f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.z, (HeimVec3f){0.0f, 0.0f, 1.0f});
    model_matrix = heim_mat4_scale(model_matrix, scale);

    heim_texture_bind(model->texture, 0);
    heim_shader_bind(model->obj->shader);
    heim_shader_set_uniform_mat4(model->obj->shader, "model", model_matrix);

    // view_matrix = heim_mat4_translate(view_matrix, (HeimVec3f){0.0f, 0.0f, -3.0f});

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

    heim_shader_set_uniform_mat4(model->obj->shader, "view", view_mat);

    heim_shader_set_uniform_mat4(model->obj->shader, "view", view_mat);

    HeimMat4 projection_matrix = heim_mat4_identity();
    projection_matrix = heim_mat4_perspective(heim_math_deg_to_rad(45.0f), 800 / 600, 0.1f, 100.0f);

    heim_shader_set_uniform_mat4(model->obj->shader, "projection", projection_matrix);

    heim_obj_render(model->obj);
}
