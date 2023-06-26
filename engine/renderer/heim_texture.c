#include "renderer/heim_texture.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"

#define MAX_TEXTURES 32

HeimTexture* heim_create_texture(char* path) {
    HeimTexture* tex = HEIM_MALLOC(HeimTexture, HEIM_MEMORY_TYPE_RENDERER);

    int w, h, channels;
    stbi_set_flip_vertically_on_load(1);
    uint8_t* texture_data = stbi_load(path, &w, &h, &channels, 0);

    if (!texture_data) {
        HEIM_LOG_ERROR("Failed to load texture: %s", path);
    }

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    if (channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    } else if (channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    } else {
        HEIM_LOG_ERROR("Unsupported number of channels: %d", channels);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texture_data);

    tex->width = w;
    tex->height = h;
    tex->channels = channels;

    return tex;
}

void heim_destroy_texture(HeimTexture* texture) {
    glDeleteTextures(1, &texture->id);
    HEIM_FREE(texture, HEIM_MEMORY_TYPE_RENDERER);
}

uint32_t heim_texture_get_id(HeimTexture* texture) {
    return texture->id;
}
uint32_t heim_texture_get_width(HeimTexture* texture) {
    return texture->width;
}
uint32_t heim_texture_get_height(HeimTexture* texture) {
    return texture->height;
}
uint32_t heim_texture_get_channels(HeimTexture* texture) {
    return texture->channels;
}
