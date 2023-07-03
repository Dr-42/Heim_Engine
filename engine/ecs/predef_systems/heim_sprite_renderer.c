#include "core/heim_windowing.h"
#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_ui_sprite.h"
#include "ecs/predef_comps/heim_uitransform.h"
#include "ecs/predef_systems/heim_ui_sprite_renderer.h"
#include "math/heim_mat.h"
#include "renderer/heim_shader.h"
#include "renderer/heim_sprite.h"

static struct {
    HeimShader* shader;
} sprite_render_system;

void heim_ui_sprite_render_system_init() {
    sprite_render_system.shader = heim_shader_create();
    heim_shader_init(sprite_render_system.shader, "assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
}

void heim_ui_sprite_render_system(HeimEntity entity, float dt) {
    (void)dt;
    bool has_sprite = heim_ecs_has_component(entity, get_ui_sprite_component());
    bool has_ui_transform = heim_ecs_has_component(entity, get_ui_transform_component());

    if (!has_sprite || !has_ui_transform) {
        return;
    }

    if (!has_ui_transform) {
        HEIM_LOG_WARN("Entity %d has ui sprite but no ui transform", entity);
    }

    HeimUiSprite* ui_sprite = heim_ecs_get_component_data(entity, get_ui_sprite_component());
    HeimUiTransform* transform = heim_ecs_get_component_data(entity, get_ui_transform_component());

    HeimMat4 projection = heim_mat4_ortho(0.0f, heim_window_get_window_size().x, heim_window_get_window_size().y, 0.0f, -1.0f, 1.0f);
    ui_sprite->sprite->projection = projection;

    HeimMat4 model = heim_mat4_identity();
    model = heim_mat4_translate(model, (HeimVec3f){transform->position.x, transform->position.y, 0.0f});
    model = heim_mat4_rotate(model, transform->rotation, (HeimVec3f){0.0f, 0.0f, 1.0f});
    model = heim_mat4_translate(model, (HeimVec3f){-0.5 * transform->size.x, 0.5 * transform->size.y, 0.0f});
    model = heim_mat4_scale(model, (HeimVec3f){transform->size.x, -1 * transform->size.y, 1.0f});

    ui_sprite->sprite->model = model;

    HeimMat4 view = heim_mat4_identity();
    ui_sprite->sprite->view = view;

    heim_shader_bind(sprite_render_system.shader);
    heim_texture_bind(ui_sprite->sprite->texture, 0);
    heim_shader_set_uniform_mat4(sprite_render_system.shader, "projection", ui_sprite->sprite->projection);
    heim_shader_set_uniform_mat4(sprite_render_system.shader, "view", ui_sprite->sprite->view);
    heim_shader_set_uniform_mat4(sprite_render_system.shader, "model", ui_sprite->sprite->model);
    heim_shader_set_uniform1i(sprite_render_system.shader, "image", 0);

    heim_sprite_render(ui_sprite->sprite);
}

void heim_ui_sprite_render_system_free() {
    heim_shader_free(sprite_render_system.shader);
}