#include <core/heim_engine.h>
#include <ecs/heim_ecs_predef.h>
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

void testbed_init() {
    entity1 = heim_ecs_create_entity();
    object1 = heim_obj_load("assets/models/backpack.obj");
    tex1 = heim_create_texture("assets/textures/backpack.jpg");

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
    tex2 = heim_create_texture("assets/textures/susan.png");

    model2 = (HeimModel){object2, tex2};
    transform2 = (HeimTransform){
        .position = {-5.0f, 3.0f, -5.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .size = {2.0f, 2.0f, 2.0f},
    };

    heim_ecs_add_component(entity2, get_model_component(), &model2);
    heim_ecs_add_component(entity2, get_transform_component(), &transform2);

    entity3 = heim_ecs_create_entity();
    tex3 = heim_create_texture("assets/textures/heim.png");
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
}

void testbed_update(float *dt) {
    (void)dt;
    if (heim_input_key_pressed(GLFW_KEY_ESCAPE)) {
        heim_engine_should_close(true);
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

    heim_destroy_texture(tex1);
    heim_obj_free(object1);

    heim_destroy_texture(tex2);
    heim_obj_free(object2);

    heim_destroy_texture(tex3);
    heim_sprite_free(sprite);

    heim_engine_shutdown();
    return 0;
}
