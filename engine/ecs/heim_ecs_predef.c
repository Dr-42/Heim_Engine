#include "ecs/heim_ecs_predef.h"

#include "ecs/heim_ecs.h"
#include "ecs/predef_comps/heim_model.h"
#include "ecs/predef_comps/heim_transform.h"
#include "ecs/predef_comps/heim_ui_sprite.h"
#include "ecs/predef_comps/heim_uitransform.h"
#include "ecs/predef_systems/heim_model_renderer.h"
#include "ecs/predef_systems/heim_ui_sprite_renderer.h"

uint64_t HEIM_COMPONENT_UI_TRANSFORM;
uint64_t HEIM_COMPONENT_TRANSFORM;
uint64_t HEIM_COMPONENT_UI_SPRITE;
uint64_t HEIM_COMPONENT_MODEL;

void heim_ecs_load_predef_components() {
    HEIM_COMPONENT_UI_TRANSFORM = heim_ecs_register_component(sizeof(HeimUiTransform));
    HEIM_COMPONENT_TRANSFORM = heim_ecs_register_component(sizeof(HeimTransform));
    HEIM_COMPONENT_UI_SPRITE = heim_ecs_register_component(sizeof(HeimUiSprite));
    HEIM_COMPONENT_MODEL = heim_ecs_register_component(sizeof(HeimModel));
}
void heim_load_predef_systems() {
    heim_ecs_add_system(heim_model_renderer_system);
    heim_ecs_add_system(heim_ui_sprite_render_system);
}

uint64_t get_ui_transform_component() {
    return HEIM_COMPONENT_UI_TRANSFORM;
}

uint64_t get_transform_component() {
    return HEIM_COMPONENT_TRANSFORM;
}

uint64_t get_ui_sprite_component() {
    return HEIM_COMPONENT_UI_SPRITE;
}

uint64_t get_model_component() {
    return HEIM_COMPONENT_MODEL;
}
