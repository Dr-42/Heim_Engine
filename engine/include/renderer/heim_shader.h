#ifndef HEIM_SHADER_H
#define HEIM_SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "math/heim_vector.h"

typedef struct HeimShader {
    GLuint program;
} HeimShader;

HeimShader* heim_shader_create();
void heim_shader_init(HeimShader* shader, const char* vertex_shader_path, const char* fragment_shader_path);
void heim_shader_free(HeimShader* shader);

void heim_shader_bind(HeimShader* shader);
void heim_shader_unbind();

uint64_t heim_shader_get_file_size(const char* file_path);
bool heim_shader_get_source(const char* file_path, char* buffer, uint64_t file_size);
uint32_t heim_shader_get_program(const char* vert_source_path, const char* frag_source_path);

bool heim_shader_read_file(const char* file_path, char* buffer);

void heim_shader_set_uniform1i(HeimShader* shader, const char* name, int value);
void heim_shader_set_uniform1f(HeimShader* shader, const char* name, float value);
void heim_shader_set_uniform2f(HeimShader* shader, const char* name, HeimVec2f value);
void heim_shader_set_uniform3f(HeimShader* shader, const char* name, HeimVec3f value);
void heim_shader_set_uniform4f(HeimShader* shader, const char* name, HeimVec4f value);

#endif  // HEIM_SHADER_H