#include "core/heim_logger.h"
#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_ui_text.h"
#include "ecs/predef_comps/heim_uitransform.h"
#include "ecs/predef_systems/heim_ui_text_renderer.h"

void heim_ui_text_render_system(HeimEntity entity, float dt) {
    (void)dt;
    bool has_ui_text = heim_ecs_has_component(entity, get_ui_text_component());
    bool has_ui_transform = heim_ecs_has_component(entity, get_ui_transform_component());

    if (!has_ui_text || !has_ui_transform) {
        return;
    }

    if (has_ui_text && !has_ui_transform) {
        HEIM_LOG_ERROR("Entity has ui_text component but not ui_transform component");
        return;
    }

    HeimUiText* ui_text = heim_ecs_get_component_data(entity, get_ui_text_component());
    HeimUiTransform* ui_transform = heim_ecs_get_component_data(entity, get_ui_transform_component());

    HeimVec3f color = (HeimVec3f){ui_transform->color.x, ui_transform->color.y, ui_transform->color.z};
    heim_font_render_text(ui_text->font, ui_text->text, ui_transform->position, ui_transform->size.x, color);
}