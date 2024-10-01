#include "core/heim_logger.h"
#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_ui_text.h"
#include "ecs/predef_comps/heim_uitransform.h"
#include "ecs/predef_systems/heim_ui_text_renderer.h"
#include "renderer/heim_shader.h"

static struct {
    HeimShader* shader;
} text_render_system;

void heim_ui_text_render_system_init() {
    text_render_system.shader = heim_shader_create();
    heim_shader_init(text_render_system.shader, "assets/shaders/text.vert", "assets/shaders/text.frag");
}

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

    HeimVec3f color = heim_vec3f_from_vec4f(ui_transform->color);
    heim_font_render_text(ui_text->font, text_render_system.shader, ui_text->text, ui_transform->position, ui_transform->size.x,
                          color);
}

void heim_ui_text_render_system_free() { heim_shader_free(text_render_system.shader); }