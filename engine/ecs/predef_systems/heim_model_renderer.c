#include "ecs/predef_systems/heim_model_renderer.h"

#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_camera.h"
#include "ecs/predef_comps/heim_world.h"
#include "math/heim_mat.h"
#include "renderer/heim_shader.h"

static struct {
    HeimShader* shader;
    HeimWorld* world;
} model_render_system;

void heim_model_renderer_init() {
    model_render_system.shader = heim_shader_create();
    heim_shader_init(model_render_system.shader, "assets/shaders/model.vert", "assets/shaders/model.frag");
}

void heim_model_render(HeimModel* model, HeimVec3f position, HeimVec3f rotation, HeimVec3f scale) {
    HeimMat4 model_matrix = heim_mat4_identity();
    model_matrix = heim_mat4_translate(model_matrix, position);
    model_matrix = heim_mat4_rotate(model_matrix, rotation.x, (HeimVec3f){1.0f, 0.0f, 0.5f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.y, (HeimVec3f){0.0f, 1.0f, 0.0f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.z, (HeimVec3f){0.0f, 0.0f, 1.0f});
    model_matrix = heim_mat4_scale(model_matrix, scale);
    HeimCamera* camera = model_render_system.world->camera;
    HeimTransform* camera_transform = model_render_system.world->camera_transform;

    heim_shader_bind(model_render_system.shader);
    heim_shader_set_uniform_mat4(model_render_system.shader, "model", model_matrix);
    // Assuming you have a HeimMat4 and HeimVec3f structures and functions defined somewhere
    HeimMat4 cameraRotation = heim_mat4_identity();  // Initialize to identity matrix

    // Update the camera rotation
    HeimVec3f yawAxis = {0.0f, 1.0f, 0.0f};
    HeimVec3f pitchAxis = {1.0f, 0.0f, 0.0f};
    cameraRotation = heim_mat4_rotate(cameraRotation, camera->yaw, yawAxis);
    cameraRotation = heim_mat4_rotate(cameraRotation, camera->pitch, pitchAxis);

    // Calculate the front vector from the camera rotation
    HeimVec3f front = heim_mat4_transform_vec3(cameraRotation, (HeimVec3f){0.0f, 0.0f, -1.0f});
    camera->front = front;

    // Calculate the up vector from the camera rotation
    HeimVec3f up = heim_mat4_transform_vec3(cameraRotation, (HeimVec3f){0.0f, 1.0f, 0.0f});
    camera->up = up;

    HeimMat4 view_matrix = heim_mat4_identity();
    view_matrix = heim_mat4_translate(view_matrix, camera_transform->position);

    // Camera has front, right and up axes defined
    view_matrix = heim_mat4_lookat(camera_transform->position, heim_vec3f_add(camera_transform->position, camera->front), camera->up);

    HeimMat4 projection_matrix = heim_mat4_perspective(camera->fov, camera->aspect, camera->near, camera->far);

    heim_shader_set_uniform_mat4(model_render_system.shader, "view", view_matrix);
    heim_shader_set_uniform_mat4(model_render_system.shader, "projection", projection_matrix);

    if (camera->render_to_texture) {
        heim_camera_bind(camera);
    }

    heim_texture_bind(model->texture, 0);
    heim_shader_set_uniform1i(model_render_system.shader, "texture_diffuse1", 0);
    heim_shader_bind(model_render_system.shader);
    heim_obj_render(model->obj);

    if (camera->render_to_texture) {
        heim_camera_unbind(camera);
    }
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

    for (size_t i = 0; i < heim_ecs_get_entity_count(); i++) {
        if (!heim_ecs_has_component(i, get_world_component())) {
            continue;
        }
        model_render_system.world = heim_ecs_get_component_data(i, get_world_component());
        break;
    }

    if (!model_render_system.world) {
        HEIM_LOG_WARN("No world found");
        //return;
    }

    heim_model_render(model, transform->position, transform->rotation, transform->size);
}
void heim_model_renderer_free() {
    heim_shader_free(model_render_system.shader);
}
