#include "ecs/heim_ecs_predef.h"

#include "ecs/heim_ecs.h"
#include "ecs/predef_comps/heim_camera.h"
#include "ecs/predef_comps/heim_model.h"
#include "ecs/predef_comps/heim_pbr_model.h"
#include "ecs/predef_comps/heim_transform.h"
#include "ecs/predef_comps/heim_ui_sprite.h"
#include "ecs/predef_comps/heim_uitransform.h"
#include "ecs/predef_systems/heim_model_renderer.h"
#include "ecs/predef_systems/heim_pbr_model_renderer.h"
#include "ecs/predef_systems/heim_skeletal_model_renderer.h"
#include "ecs/predef_systems/heim_ui_sprite_renderer.h"
#include "ecs/predef_systems/heim_ui_text_renderer.h"

HeimComponent HEIM_COMPONENT_WORLD;
HeimComponent HEIM_COMPONENT_UI_TRANSFORM;
HeimComponent HEIM_COMPONENT_TRANSFORM;
HeimComponent HEIM_COMPONENT_UI_SPRITE;
HeimComponent HEIM_COMPONENT_UI_TEXT;
HeimComponent HEIM_COMPONENT_PBR_MODEL;
HeimComponent HEIM_COMPONENT_SKELETAL_MODEL;
HeimComponent HEIM_COMPONENT_MODEL;
HeimComponent HEIM_COMPONENT_CAMERA;

void heim_ecs_load_predef_components() {
    HEIM_COMPONENT_WORLD = heim_ecs_register_component();
    HEIM_COMPONENT_UI_TRANSFORM = heim_ecs_register_component();
    HEIM_COMPONENT_TRANSFORM = heim_ecs_register_component();
    HEIM_COMPONENT_UI_SPRITE = heim_ecs_register_component();
    HEIM_COMPONENT_UI_TEXT = heim_ecs_register_component();
    HEIM_COMPONENT_MODEL = heim_ecs_register_component();
    HEIM_COMPONENT_PBR_MODEL = heim_ecs_register_component();
    HEIM_COMPONENT_SKELETAL_MODEL = heim_ecs_register_component();
    HEIM_COMPONENT_CAMERA = heim_ecs_register_component();
}

void heim_load_predef_systems() {
    heim_pbr_model_renderer_init();
    heim_skeletal_model_renderer_init();
    heim_model_renderer_init();
    heim_ui_sprite_render_system_init();
    heim_ui_text_render_system_init();
    heim_ecs_register_system(heim_pbr_model_renderer_system);
    heim_ecs_register_system(heim_skeletal_model_renderer_system);
    heim_ecs_register_system(heim_model_renderer_system);
    heim_ecs_register_system(heim_ui_sprite_render_system);
    heim_ecs_register_system(heim_ui_text_render_system);
}

void heim_unload_predef_systems() {
    heim_ui_text_render_system_free();
    heim_ui_sprite_render_system_free();
    heim_model_renderer_free();
    heim_pbr_model_renderer_free();
    heim_skeletal_model_renderer_free();
}

uint64_t get_world_component() {
    return HEIM_COMPONENT_WORLD;
}

uint64_t get_ui_transform_component() {
    return HEIM_COMPONENT_UI_TRANSFORM;
}

HeimComponent get_transform_component() {
    return HEIM_COMPONENT_TRANSFORM;
}

HeimComponent get_ui_sprite_component() {
    return HEIM_COMPONENT_UI_SPRITE;
}

HeimComponent get_ui_text_component() {
    return HEIM_COMPONENT_UI_TEXT;
}

HeimComponent get_model_component() {
    return HEIM_COMPONENT_MODEL;
}
HeimComponent get_pbr_model_component() {
    return HEIM_COMPONENT_PBR_MODEL;
}

HeimComponent get_skeletal_model_component() {
    return HEIM_COMPONENT_SKELETAL_MODEL;
}

HeimComponent get_camera_component() {
    return HEIM_COMPONENT_CAMERA;
}
