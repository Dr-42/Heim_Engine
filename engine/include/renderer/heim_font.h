#pragma once

#include <stdint.h>

#include "math/heim_vec.h"
#include "renderer/heim_shader.h"

typedef struct HeimCharacter {
    uint32_t texture_id;
    HeimVec2f size;
    HeimVec2f bearing;
    uint32_t advance;
    HeimVec2f tex_coords_start;
    HeimVec2f tex_coords_end;
} HeimCharacter;

typedef struct HeimFont {
    HeimCharacter characters[128];
    uint32_t vao;
    uint32_t vbo;
    HeimVec2f size;
    float atlas_width;
} HeimFont;

HeimFont* heim_font_create(HeimVec2f size);
void heim_font_free(HeimFont* font);

void heim_font_init(HeimFont* font, char* path, uint32_t size);
void heim_font_render_text(HeimFont* font, HeimShader* shader, char* text, HeimVec2f position, float scale, HeimVec3f color);