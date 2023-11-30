#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct HeimTexture {
    uint32_t id;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
} HeimTexture;

HeimTexture* heim_texture_create(char* path);

HeimTexture* heim_texture_create_empty_slot(uint32_t width, uint32_t height, uint32_t channels);
HeimTexture* heim_texture_create_color(uint8_t* color);
void heim_texture_free(HeimTexture* texture);

uint32_t heim_texture_get_id(HeimTexture* texture);
uint32_t heim_texture_get_width(HeimTexture* texture);
uint32_t heim_texture_get_height(HeimTexture* texture);
uint32_t heim_texture_get_channels(HeimTexture* texture);

void heim_texture_bind(HeimTexture* texture, uint32_t slot);
