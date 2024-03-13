#include <core/heim_engine.h>
#include <ecs/heim_ecs_predef.h>
#include <ecs/predef_comps/heim_all_predef_comps.h>
#include <math/heim_math_common.h>
#include <renderer/heim_animator.h>
#include <renderer/heim_font.h>
#include <renderer/heim_skeletal_model.h>
#include <renderer/heim_skybox.h>
#include <renderer/heim_sprite.h>
#include <renderer/heim_texture.h>
#include <stdint.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

HeimEntity world_entity;
HeimWorld world;
HeimSkybox *skybox;
HeimEntity camera_entity;
HeimCamera *camera;
HeimTransform camera_transform = {
    .position = (HeimVec3f){0.0f, 0.0f, 3.0f},
};
HeimEntity entity1, entity2, entity3, entity4, fps_entity;
HeimModel *object1, *object2, *object3, *object4;
HeimTexture *tex1, *tex2, *tex3, *tex1normal, *tex1metallic, *tex1roughness, *tex1ao;
HeimPBRModel model1, model2, model3, model4;
HeimTransform transform1, transform2, transform3;
HeimSprite *sprite;
HeimUiTransform ui_transform;
HeimUiSprite ui_sprite;

HeimTexture *brun_albedo, *brun_normal, *brun_metallic, *brun_roughness, *brun_ao;
HeimTexture *maria_albedo, *maria_normal, *maria_metallic, *maria_roughness, *maria_ao;
HeimTexture *cube_albedo, *cube_normal, *cube_metallic, *cube_roughness, *cube_ao;

HeimEntity camera_surface_entity;
HeimSprite *camera_surface_sprite;
HeimUiSprite camera_surface_ui_sprite;
HeimUiTransform camera_surface_transform = {
    .position = {(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2},
    .size = (HeimVec2f){(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT},
};

HeimFont *font;
HeimUiTransform fps_transform;
HeimUiText fps_text;
char text[256];

HeimSkeletalModel *skeletal_model;
HeimAnimator *animator;
HeimTransform skeletal_transform;
HeimEntity skeletal_entity;
HeimSkeletalModelComp skeletal_model_comp;

void testbed_init() {
    world_entity = heim_ecs_create_entity();
    skybox = heim_skybox_create("assets/hdr/studio.hdr");
    entity1 = heim_ecs_create_entity();
    object1 = heim_model_create("assets/models/Brunhilda1.obj");
    tex1 = heim_texture_create("assets/textures/brunhilda/Brunhild_diffuse2.png");
    uint8_t white[4] = {255, 255, 255, 255};
    tex1normal = heim_texture_create_color(white);
    tex1metallic = heim_texture_create_color(white);
    tex1roughness = heim_texture_create_color(white);
    tex1ao = heim_texture_create_color(white);

    heim_model_set_albedo(object1, tex1);
    heim_model_set_normal(object1, tex1normal);
    heim_model_set_specular(object1, tex1metallic);
    heim_model_set_roughness(object1, tex1roughness);
    heim_model_set_ao(object1, tex1ao);

    camera_entity = heim_ecs_create_entity();
    camera = heim_camera_new(
        80.0f,
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f,
        100.0f,
        true,
        WINDOW_WIDTH, WINDOW_HEIGHT);
    camera->clear_color = (HeimVec4f){0.1f, 0.1f, 0.1f, 1.0f};
    camera->front = (HeimVec3f){0.0f, 0.0f, -1.0f};
    camera->up = (HeimVec3f){0.0f, 1.0f, 0.0f};
    heim_ecs_add_component(camera_entity, get_camera_component(), camera);
    heim_ecs_add_component(camera_entity, get_transform_component(), &camera_transform);
    world = (HeimWorld){
        .skybox = skybox,
        .camera = camera,
        .camera_transform = &camera_transform,
    };

    heim_ecs_add_component(world_entity, get_world_component(), &world);

    model1 = (HeimPBRModel){
        .model = object1,
    };
    transform1 = (HeimTransform){
        .position = {-0.5f, -0.5f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .size = {1.0f, 1.0f, 1.0f},
    };

    heim_ecs_add_component(entity1, get_pbr_model_component(), &model1);
    heim_ecs_add_component(entity1, get_transform_component(), &transform1);

    entity2 = heim_ecs_create_entity();
    tex2 = heim_texture_create("assets/textures/susan.png");
    object2 = heim_model_create("assets/models/Brunhilda1.obj");

    brun_albedo = heim_texture_create("assets/textures/brunhilda/Brunhild_diffuse2.png");
    brun_normal = heim_texture_create("assets/textures/brunhilda/Brunhilda_normals.png");
    brun_metallic = heim_texture_create("assets/textures/brunhilda/DefaultMaterial_Metallic.png");
    brun_roughness = heim_texture_create("assets/textures/brunhilda/DefaultMaterial_Roughness.png");
    brun_ao = heim_texture_create("assets/textures/brunhilda/DefaultMaterial_Mixed_AO.png");

    heim_model_set_albedo(object2, brun_albedo);
    heim_model_set_normal(object2, brun_normal);
    heim_model_set_specular(object2, brun_metallic);
    heim_model_set_roughness(object2, brun_roughness);
    heim_model_set_ao(object2, brun_ao);

    entity4 = heim_ecs_create_entity();
    object4 = heim_model_create("assets/models/Maria.fbx");

    maria_albedo = heim_texture_create("assets/textures/Maria/maria_diffuse.png");
    maria_normal = heim_texture_create("assets/textures/Maria/maria_normal.png");
    maria_metallic = heim_texture_create("assets/textures/Maria/maria_specular.png");
    maria_roughness = heim_texture_create("assets/textures/Maria/maria_height.png");
    maria_ao = heim_texture_create("assets/textures/Maria/maria_ao.png");

    heim_model_set_albedo(object4, maria_albedo);
    heim_model_set_normal(object4, maria_normal);
    heim_model_set_specular(object4, maria_metallic);
    heim_model_set_roughness(object4, maria_roughness);
    heim_model_set_ao(object4, maria_ao);

    object3 = heim_model_create("assets/models/cube.obj");

    cube_albedo = heim_texture_create("assets/textures/rust/albedo.png");
    cube_normal = heim_texture_create("assets/textures/rust/normal.png");
    cube_metallic = heim_texture_create("assets/textures/rust/metallic.png");
    cube_roughness = heim_texture_create("assets/textures/rust/roughness.png");
    cube_ao = heim_texture_create("assets/textures/rust/ao.png");

    heim_model_set_albedo(object3, cube_albedo);
    heim_model_set_normal(object3, cube_normal);
    heim_model_set_specular(object3, cube_metallic);
    heim_model_set_roughness(object3, cube_roughness);
    heim_model_set_ao(object3, cube_ao);

    model2 = (HeimPBRModel){
        .model = object2,
    };

    model3 = (HeimPBRModel){
        .model = object3,
    };

    model4 = (HeimPBRModel){
        .model = object4,
    };

    transform2 = (HeimTransform){
        .position = {0.5f, -0.5f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .size = {1.0f, 1.0f, 1.0f},
    };

    transform3 = (HeimTransform){
        .position = {1.5f, -0.5f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .size = {0.007f, 0.007f, 0.007f},
    };
    skeletal_transform = (HeimTransform){
        .position = {2.5f, -0.5f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .size = {0.007f, 0.007f, 0.007f},
    };

    heim_ecs_add_component(entity2, get_pbr_model_component(), &model2);
    heim_ecs_add_component(entity2, get_transform_component(), &transform2);

    heim_ecs_add_component(entity4, get_pbr_model_component(), &model4);
    heim_ecs_add_component(entity4, get_transform_component(), &transform3);

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

    camera_surface_entity = heim_ecs_create_entity();
    camera_surface_sprite = heim_create_sprite(camera->texture);
    camera_surface_ui_sprite = (HeimUiSprite){
        .sprite = camera_surface_sprite,
    };
    heim_ecs_add_component(camera_surface_entity, get_ui_sprite_component(), &camera_surface_ui_sprite);
    heim_ecs_add_component(camera_surface_entity, get_ui_transform_component(), &camera_surface_transform);

    fps_entity = heim_ecs_create_entity();
    font = heim_font_create((HeimVec2f){WINDOW_WIDTH, WINDOW_HEIGHT});
    heim_font_init(font, "assets/fonts/typewriter.ttf", 32);
    fps_transform = (HeimUiTransform){
        .position = {10.0f, 30.0f},
        .size = {1.0f, 1.0f},
        .color = (HeimVec4f){0.4f, 0.2f, 0.4f, 1.0f},
    };

    fps_text = (HeimUiText){
        .font = font,
        .text = text,
    };

    heim_ecs_add_component(fps_entity, get_ui_transform_component(), &fps_transform);
    heim_ecs_add_component(fps_entity, get_ui_text_component(), &fps_text);

    skeletal_entity = heim_ecs_create_entity();
    skeletal_model = heim_skeletal_model_create("assets/models/Maria.fbx");
    heim_skeletal_model_set_albedo(skeletal_model, maria_albedo);
    heim_skeletal_model_set_normal(skeletal_model, maria_normal);
    heim_skeletal_model_set_specular(skeletal_model, maria_metallic);
    heim_skeletal_model_set_roughness(skeletal_model, maria_roughness);
    heim_skeletal_model_set_ao(skeletal_model, maria_ao);
    animator = heim_animator_init("assets/models/Maria.fbx", skeletal_model);

    skeletal_model_comp = (HeimSkeletalModelComp){
        .model = skeletal_model,
        .animator = animator,
    };

    heim_ecs_add_component(skeletal_entity, get_skeletal_model_component(), &skeletal_model_comp);
    heim_ecs_add_component(skeletal_entity, get_transform_component(), &skeletal_transform);
}

float speed = 2.0f;
float sensitivity = 2.0f;
uint64_t frame_count = 0;

void testbed_update(float dt) {
    if (frame_count % 10 == 0) {
        sprintf(text, "Heim Engine - FPS: %.2f", 1.0f / dt);
    }

    if (heim_input_key_pressed(GLFW_KEY_ESCAPE)) {
        heim_engine_should_close(true);
    }

    if (heim_input_key_down(GLFW_KEY_LEFT_SHIFT)) {
        if (heim_ecs_has_component(entity2, get_pbr_model_component())) {
            if (heim_ecs_get_component_data(entity2, get_pbr_model_component()) == &model2) {
                heim_ecs_remove_component(entity2, get_pbr_model_component());
                heim_ecs_add_component(entity2, get_pbr_model_component(), &model3);
            }
        }
    }

    if (heim_input_key_up(GLFW_KEY_LEFT_SHIFT)) {
        if (heim_ecs_has_component(entity2, get_pbr_model_component())) {
            if (heim_ecs_get_component_data(entity2, get_pbr_model_component()) == &model3) {
                heim_ecs_remove_component(entity2, get_pbr_model_component());
                heim_ecs_add_component(entity2, get_pbr_model_component(), &model2);
            }
        }
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
    if (frame_count) {
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
            speed = heim_math_clamp(speed, 0.01f, 10.0f);
        }

        if (heim_input_key_pressed(GLFW_KEY_DOWN)) {
            speed -= 0.1f;
            speed = heim_math_clamp(speed, 0.01f, 10.0f);
        }

        if (heim_input_key_pressed(GLFW_KEY_LEFT)) {
            sensitivity -= 0.01f;
            sensitivity = heim_math_clamp(sensitivity, 0.01f, 10.0f);
        }

        if (heim_input_key_pressed(GLFW_KEY_RIGHT)) {
            sensitivity += 0.01f;
            sensitivity = heim_math_clamp(sensitivity, 0.01f, 10.0f);
        }

        if (heim_input_key_pressed(GLFW_KEY_F)) {
            heim_engine_set_fullscreen(!heim_engine_is_fullscreen());
        }
    }

    // Switch animations
    if (heim_input_key_pressed(GLFW_KEY_1)) {
        heim_animator_switch_animation(animator, 0);
    } else if (heim_input_key_pressed(GLFW_KEY_2)) {
        heim_animator_switch_animation(animator, 1);
    } else if (heim_input_key_pressed(GLFW_KEY_3)) {
        heim_animator_switch_animation(animator, 2);
    } else if (heim_input_key_pressed(GLFW_KEY_4)) {
        heim_animator_switch_animation(animator, 3);
    } else if (heim_input_key_pressed(GLFW_KEY_5)) {
        heim_animator_switch_animation(animator, 4);
    } else if (heim_input_key_pressed(GLFW_KEY_6)) {
        heim_animator_switch_animation(animator, 5);
    } else if (heim_input_key_pressed(GLFW_KEY_7)) {
        heim_animator_switch_animation(animator, 6);
    } else if (heim_input_key_pressed(GLFW_KEY_8)) {
        heim_animator_switch_animation(animator, 7);
    } else if (heim_input_key_pressed(GLFW_KEY_9)) {
        heim_animator_switch_animation(animator, 8);
    } else if (heim_input_key_pressed(GLFW_KEY_0)) {
        heim_animator_switch_animation(animator, 9);
    }

    frame_count++;
}

void testbed_free() {
    heim_skybox_destroy(skybox);
    heim_texture_free(tex1);
    heim_texture_free(tex1normal);
    heim_texture_free(tex1metallic);
    heim_texture_free(tex1roughness);
    heim_texture_free(tex1ao);
    heim_model_destroy(object1);

    heim_texture_free(tex2);
    heim_model_destroy(object2);
    heim_model_destroy(object3);
    heim_model_destroy(object4);

    heim_texture_free(tex3);
    heim_sprite_free(sprite);

    heim_camera_free(camera);
    heim_sprite_free(camera_surface_sprite);

    heim_texture_free(brun_albedo);
    heim_texture_free(brun_normal);
    heim_texture_free(brun_metallic);
    heim_texture_free(brun_roughness);
    heim_texture_free(brun_ao);

    heim_texture_free(maria_albedo);
    heim_texture_free(maria_normal);
    heim_texture_free(maria_metallic);
    heim_texture_free(maria_roughness);
    heim_texture_free(maria_ao);

    heim_texture_free(cube_albedo);
    heim_texture_free(cube_normal);
    heim_texture_free(cube_metallic);
    heim_texture_free(cube_roughness);
    heim_texture_free(cube_ao);

    heim_skeletal_model_destroy(skeletal_model);
    heim_animator_free(animator);

    heim_font_free(font);
}

int main(void) {
    heim_engine_new("Heim Engine Testbed");
    heim_engine_set_window_size(WINDOW_WIDTH, WINDOW_HEIGHT);
    heim_engine_set_window_top_left(0, 0);

    heim_engine_init(testbed_init);
    heim_input_mouse_hide(true);
    heim_input_mouse_grab(true);
    heim_engine_set_fullscreen(true);
    heim_engine_run(testbed_update);

    heim_engine_shutdown(testbed_free);
    return 0;
}
