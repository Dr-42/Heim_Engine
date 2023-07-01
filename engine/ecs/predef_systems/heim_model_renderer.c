#include "ecs/predef_systems/heim_model_renderer.h"

#include "core/heim_input.h"
#include "core/heim_windowing.h"
#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_camera.h"
#include "math/heim_mat.h"
#include "math/heim_math_common.h"

void heim_model_render(HeimModel* model, HeimVec3f position, HeimVec3f rotation, HeimVec3f scale, HeimCamera* camera, HeimTransform* camera_transform) {
    HeimMat4 model_matrix = heim_mat4_identity();
    model_matrix = heim_mat4_translate(model_matrix, position);
    model_matrix = heim_mat4_rotate(model_matrix, rotation.x, (HeimVec3f){1.0f, 0.0f, 0.5f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.y, (HeimVec3f){0.0f, 1.0f, 0.0f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.z, (HeimVec3f){0.0f, 0.0f, 1.0f});
    model_matrix = heim_mat4_scale(model_matrix, scale);

    heim_texture_bind(model->texture, 0);
    heim_shader_bind(model->obj->shader);
    heim_shader_set_uniform_mat4(model->obj->shader, "model", model_matrix);
    camera->front.x = -heim_math_sin(camera->yaw) * heim_math_cos(camera->pitch);
    camera->front.y = -heim_math_sin(camera->pitch);
    camera->front.z = -heim_math_cos(camera->yaw) * heim_math_cos(camera->pitch);

    camera->front = heim_vec3f_normalize(camera->front);

    HeimVec3f right;
    right.x = -heim_math_cos(camera->yaw);
    right.y = 0.0f;
    right.z = heim_math_sin(camera->yaw);

    camera->up = heim_vec3f_normalize(heim_vec3f_cross(right, camera->front));
    camera->up.y *= -1.0f;

    HeimMat4 view_matrix = heim_mat4_identity();
    view_matrix = heim_mat4_translate(view_matrix, camera_transform->position);

    // Camera has front, right and up axes defined
    view_matrix = heim_mat4_lookat(camera_transform->position, heim_vec3f_add(camera_transform->position, camera->front), camera->up);

    HeimMat4 projection_matrix = heim_mat4_perspective(camera->fov, camera->aspect, camera->near, camera->far);

    heim_shader_set_uniform_mat4(model->obj->shader, "view", view_matrix);
    heim_shader_set_uniform_mat4(model->obj->shader, "projection", projection_matrix);

    heim_obj_render(model->obj);
}

void heim_model_renderer_system(HeimEntity entity, float dt) {
    (void)dt;
    bool has_model = heim_ecs_has_component(entity, get_model_component());
    bool has_transform = heim_ecs_has_component(entity, get_transform_component());

    if (!has_model || !has_transform) {
        return;
    }

    if (has_model && !has_transform) {
        HEIM_LOG_WARN("Entity has model component but not transform component");
        return;
    }

    HeimModel* model = heim_ecs_get_component_data(entity, get_model_component());
    HeimTransform* transform = heim_ecs_get_component_data(entity, get_transform_component());

    HeimEntity camera = 0;

    for (size_t i = 0; i < MAX_ENTITIES; i++) {
        if (!heim_ecs_has_component(i, get_camera_component())) {
            continue;
        }

        camera = i;
        break;
    }

    if (!camera) {
        HEIM_LOG_WARN("No camera found");
        return;
    }

    HeimCamera* camera_data = heim_ecs_get_component_data(camera, get_camera_component());
    HeimTransform* camera_transform = heim_ecs_get_component_data(camera, get_transform_component());

    heim_model_render(model, transform->position, transform->rotation, transform->size, camera_data, camera_transform);
}