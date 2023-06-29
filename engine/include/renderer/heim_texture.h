#ifndef HEIM_TEXTURE_H
#define HEIM_TEXTURE_H

#include <stdint.h>

typedef struct HeimTexture {
    uint32_t id;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
} HeimTexture;

HeimTexture* heim_create_texture(char* path);
void heim_destroy_texture(HeimTexture* texture);

uint32_t heim_texture_get_id(HeimTexture* texture);
uint32_t heim_texture_get_width(HeimTexture* texture);
uint32_t heim_texture_get_height(HeimTexture* texture);
uint32_t heim_texture_get_channels(HeimTexture* texture);

void heim_texture_bind(HeimTexture* texture, uint32_t slot);

#endif  // HEIM_TEXTURE_H