#include <core/heim_engine.h>
#include <ecs/heim_ecs_predef.h>
#include <ecs/predef_comps/heim_camera.h>
#include <ecs/predef_comps/heim_model.h>
#include <ecs/predef_comps/heim_transform.h>
#include <ecs/predef_comps/heim_ui_sprite.h>
#include <ecs/predef_comps/heim_uitransform.h>
#include <math/heim_math_common.h>
#include <renderer/heim_obj.h>
#include <renderer/heim_sprite.h>
#include <renderer/heim_texture.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

float total_time = 0.0f;

HeimEntity entity1, entity2, entity3;
HeimObj *object1, *object2;
HeimTexture *tex1, *tex2, *tex3;
HeimModel model1, model2;
HeimTransform transform1, transform2;
HeimSprite *sprite;
HeimUiTransform ui_transform;
HeimUiSprite ui_sprite;

HeimEntity camera_entity;
HeimCamera *camera;
HeimTransform camera_transform = {
    .position = {0.0f, 0.0f, 3.0f},
};

HeimEntity camera_surface_entity;
HeimSprite *camera_surface_sprite;
HeimUiSprite camera_surface_ui_sprite;
HeimUiTransform camera_surface_transform = {
    .position = {(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2},
    .size = (HeimVec2f){(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT},
};

void testbed_init() {
    entity1 = heim_ecs_create_entity();
    object1 = heim_obj_load("assets/models/backpack.obj");
    tex1 = heim_texture_create("assets/textures/backpack.jpg");

    model1 = (HeimModel){object1, tex1};
    transform1 = (HeimTransform){
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .size = {1.0f, 1.0f, 1.0f},
    };

    heim_ecs_add_component(entity1, get_model_component(), &model1);
    heim_ecs_add_component(entity1, get_transform_component(), &transform1);

    entity2 = heim_ecs_create_entity();
    object2 = heim_obj_load("assets/models/susan.obj");
    tex2 = heim_texture_create("assets/textures/susan.png");

    model2 = (HeimModel){object2, tex2};
    transform2 = (HeimTransform){
        .position = {-5.0f, 3.0f, -5.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .size = {2.0f, 2.0f, 2.0f},
    };

    heim_ecs_add_component(entity2, get_model_component(), &model2);
    heim_ecs_add_component(entity2, get_transform_component(), &transform2);

    entity3 = heim_ecs_create_entity();
    tex3 = heim_texture_create("./assets/textures/Heim.png");
    sprite = heim_create_sprite(tex3);
    ui_transform = (HeimUiTransform){
        .position = {WINDOW_WIDTH - 75.0f, WINDOW_HEIGHT - 75.0f},
        .size = {150.0f, 150.0f},
    };

    ui_sprite = (HeimUiSprite){
        .sprite = sprite,
    };

    heim_ecs_add_component(entity3, get_ui_transform_component(), &ui_transform);
    heim_ecs_add_component(entity3, get_ui_sprite_component(), &ui_sprite);

    camera_entity = heim_ecs_create_entity();
    camera = heim_camera_new(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f, true, WINDOW_WIDTH, WINDOW_HEIGHT);
    camera->clear_color = (HeimVec4f){0.2f, 0.3f, 0.3f, 1.0f};
    heim_ecs_add_component(camera_entity, get_camera_component(), camera);
    heim_ecs_add_component(camera_entity, get_transform_component(), &camera_transform);

    camera_surface_entity = heim_ecs_create_entity();
    camera_surface_sprite = heim_create_sprite(camera->texture);
    camera_surface_ui_sprite = (HeimUiSprite){
        .sprite = camera_surface_sprite,
    };
    heim_ecs_add_component(camera_surface_entity, get_ui_sprite_component(), &camera_surface_ui_sprite);
    heim_ecs_add_component(camera_surface_entity, get_ui_transform_component(), &camera_surface_transform);
}

float speed = 3.0f;
float sensitivity = 3.0f;

void testbed_update(float dt) {
    (void)dt;
    if (heim_input_key_pressed(GLFW_KEY_ESCAPE)) {
        heim_engine_should_close(true);
    }

    if (heim_input_key_pressed(GLFW_KEY_W)) {
        camera_transform.position = heim_vec3f_add(
            camera_transform.position,
            heim_vec3f_mul(heim_vec3f_normalize(camera->front), speed * dt));
    }

    if (heim_input_key_pressed(GLFW_KEY_S)) {
        camera_transform.position = heim_vec3f_sub(
            camera_transform.position,
            heim_vec3f_mul(heim_vec3f_normalize(camera->front), speed * dt));
    }

    if (heim_input_key_pressed(GLFW_KEY_A)) {
        camera_transform.position = heim_vec3f_sub(
            camera_transform.position,
            heim_vec3f_mul(heim_vec3f_normalize(heim_vec3f_cross(camera->front, camera->up)),
                           speed * dt));
    }

    if (heim_input_key_pressed(GLFW_KEY_D)) {
        camera_transform.position = heim_vec3f_add(
            camera_transform.position,
            heim_vec3f_mul(heim_vec3f_normalize(heim_vec3f_cross(camera->front, camera->up)),
                           speed * dt));
    }

    if (heim_input_key_pressed(GLFW_KEY_Q)) {
        camera_transform.position = heim_vec3f_add(
            camera_transform.position, heim_vec3f_mul(camera->up, speed * dt));
    }

    if (heim_input_key_pressed(GLFW_KEY_E)) {
        camera_transform.position = heim_vec3f_sub(
            camera_transform.position, heim_vec3f_mul(camera->up, speed * dt));
    }

    HeimVec2f mouseDelta = heim_input_mouse_delta();

    // Reorient camera without yaw. Readjust the up and front vectors.

    if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f || mouseDelta.x < 0.5f || mouseDelta.y < 0.5f) {
        camera->yaw -= mouseDelta.x * sensitivity * dt;
        camera->pitch -= mouseDelta.y * sensitivity * dt;
    }

    if (heim_input_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
        camera->yaw = 0.0f;
        camera->pitch = 0.0f;

        camera->front = (HeimVec3f){0.0f, 0.0f, -1.0f};
        camera->up = (HeimVec3f){0.0f, 1.0f, 0.0f};
        camera_transform.position = (HeimVec3f){0.0f, 0.0f, 3.0f};
    }

    if (heim_input_key_pressed(GLFW_KEY_UP)) {
        speed += 0.1f;
    }

    if (heim_input_key_pressed(GLFW_KEY_DOWN)) {
        speed -= 0.1f;
    }

    if (heim_input_key_pressed(GLFW_KEY_LEFT)) {
        sensitivity -= 0.01f;
    }

    if (heim_input_key_pressed(GLFW_KEY_RIGHT)) {
        sensitivity += 0.01f;
    }

    if (heim_input_key_pressed(GLFW_KEY_F)) {
        heim_engine_set_fullscreen(!heim_engine_is_fullscreen());
    }
}

int main(void) {
    heim_engine_new("Heim Engine Testbed");
    heim_engine_set_window_size(WINDOW_WIDTH, WINDOW_HEIGHT);
    heim_engine_set_window_top_left(100, 100);

    heim_engine_init(testbed_init);
    heim_input_mouse_hide(true);
    heim_input_mouse_grab(true);
    heim_engine_run(testbed_update);

    heim_texture_free(tex1);
    heim_obj_free(object1);

    heim_texture_free(tex2);
    heim_obj_free(object2);

    heim_texture_free(tex3);
    heim_sprite_free(sprite);

    heim_camera_free(camera);
    heim_sprite_free(camera_surface_sprite);

    heim_engine_shutdown();
    return 0;
}
