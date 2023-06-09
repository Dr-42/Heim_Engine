#include "ecs/predef_systems/heim_pbr_model_renderer.h"

#include "core/heim_input.h"
#include "core/heim_windowing.h"
#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_camera.h"
#include "math/heim_mat.h"
#include "math/heim_math_common.h"
#include "renderer/heim_skybox.h"

float total_time = 0.0f;

static struct {
    HeimShader* shader;
    HeimSkybox* skybox;
    HeimCamera* camera;
    HeimTransform* camera_transform;
} pbr_model_render_system;

void heim_pbr_model_renderer_init(const char* skybox_path) {
    pbr_model_render_system.shader = heim_shader_create();
    heim_shader_init(pbr_model_render_system.shader, "assets/shaders/model_pbr.vert", "assets/shaders/model_pbr.frag");
    pbr_model_render_system.skybox = heim_skybox_create(skybox_path);
}

void heim_pbr_model_render(HeimPBRModel* model, HeimVec3f position, HeimVec3f rotation, HeimVec3f scale, HeimCamera* camera, HeimTransform* camera_transform, float dt) {
    HeimMat4 model_matrix = heim_mat4_identity();
    model_matrix = heim_mat4_translate(model_matrix, position);
    model_matrix = heim_mat4_rotate(model_matrix, rotation.x, (HeimVec3f){1.0f, 0.0f, 0.5f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.y, (HeimVec3f){0.0f, 1.0f, 0.0f});
    model_matrix = heim_mat4_rotate(model_matrix, rotation.z, (HeimVec3f){0.0f, 0.0f, 1.0f});
    model_matrix = heim_mat4_scale(model_matrix, scale);

    heim_shader_bind(pbr_model_render_system.shader);
    heim_shader_set_uniform_mat4(pbr_model_render_system.shader, "model", model_matrix);
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

    heim_shader_set_uniform_mat4(pbr_model_render_system.shader, "view", view_matrix);
    heim_shader_set_uniform_mat4(pbr_model_render_system.shader, "projection", projection_matrix);

    if (camera->render_to_texture) {
        heim_camera_bind(camera);
    }

    heim_texture_bind(model->albedoMap, 3);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "albedoMap", 3);
    heim_texture_bind(model->normalMap, 4);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "normalMap", 4);
    heim_texture_bind(model->metallicMap, 5);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "metallicMap", 5);
    heim_texture_bind(model->roughnessMap, 6);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "roughnessMap", 6);
    heim_texture_bind(model->aoMap, 7);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "aoMap", 7);

    heim_shader_set_uniform3f(pbr_model_render_system.shader, "camPos", camera_transform->position);

    // get time from time.h
    total_time += dt;

    HeimVec3f lightPos[] = {
        // rotate light in circle around origin
        (HeimVec3f){0.0f, 0.0f, 5.0f},
        (HeimVec3f){sin(total_time) * 5.0f, 0.0f, cos(total_time) * 5.0f},
    };

    HeimVec3f lightColors[] = {
        (HeimVec3f){150.0f, 150.0f, 150.0f},
        (HeimVec3f){50.0f, 40.0f, 00.0f},
    };

    heim_shader_set_uniform3f(pbr_model_render_system.shader, "lightPositions[0]", lightPos[0]);
    heim_shader_set_uniform3f(pbr_model_render_system.shader, "lightPositions[1]", lightPos[1]);
    heim_shader_set_uniform3f(pbr_model_render_system.shader, "lightColors[0]", lightColors[0]);
    heim_shader_set_uniform3f(pbr_model_render_system.shader, "lightColors[1]", lightColors[1]);

    // set normal matrix
    HeimMat3 normal_matrix = heim_mat3_transpose(heim_mat3_from_mat4(model_matrix));
    heim_shader_set_uniform_mat3(pbr_model_render_system.shader, "normalMatrix", normal_matrix);

    heim_skybox_bind(pbr_model_render_system.skybox);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "irradianceMap", 0);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "prefilterMap", 1);
    heim_shader_set_uniform1i(pbr_model_render_system.shader, "brdfLUT", 2);
    heim_obj_render(model->obj);

    heim_skybox_render_background(pbr_model_render_system.skybox, view_matrix, projection_matrix);
    if (camera->render_to_texture) {
        heim_camera_unbind(camera);
    }
}

void heim_pbr_model_renderer_system(HeimEntity entity, float dt) {
    (void)dt;
    bool has_model = heim_ecs_has_component(entity, get_pbr_model_component());
    bool has_transform = heim_ecs_has_component(entity, get_transform_component());

    if (!has_model || !has_transform) {
        return;
    }

    if (has_model && !has_transform) {
        HEIM_LOG_WARN("Entity has model component but not transform component");
        return;
    }

    HeimPBRModel* model = heim_ecs_get_component_data(entity, get_pbr_model_component());
    HeimTransform* transform = heim_ecs_get_component_data(entity, get_transform_component());

    HeimEntity camera = 0;
    if (pbr_model_render_system.camera == NULL) {
        for (size_t i = 0; i < heim_ecs_get_entity_count(); i++) {
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
        pbr_model_render_system.camera = heim_ecs_get_component_data(camera, get_camera_component());
        pbr_model_render_system.camera_transform = heim_ecs_get_component_data(camera, get_transform_component());
    }

    if (model && transform) {
        heim_pbr_model_render(model, transform->position, transform->rotation, transform->size, pbr_model_render_system.camera, pbr_model_render_system.camera_transform, dt);
    }
}

void heim_pbr_model_renderer_free() {
    heim_shader_free(pbr_model_render_system.shader);
    heim_skybox_destroy(pbr_model_render_system.skybox);
}