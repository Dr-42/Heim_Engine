#include "renderer/heim_shader.h"

#define HEIM_SHADER_SOURCE_MAX_LENGTH 1024

HeimShader* heim_shader_create() {
    HeimShader* shader = HEIM_MALLOC(HeimShader, HEIM_MEMORY_TYPE_RENDERER);
    shader->program = 0;
    return shader;
}

void heim_shader_init(HeimShader* shader, const char* vert_source_path, const char* frag_source_path) {
    shader->program = heim_shader_get_program(vert_source_path, frag_source_path);
    if (shader->program == (uint32_t)-1) {
        HEIM_LOG_ERROR("Failed to create shader program");
    }
}

void heim_shader_free(HeimShader* shader) {
    glDeleteProgram(shader->program);
    HEIM_FREE(shader, HEIM_MEMORY_TYPE_RENDERER);
}

void heim_shader_bind(HeimShader* shader) {
    glUseProgram(shader->program);
}

void heim_shader_unbind() {
    glUseProgram(0);
}

uint64_t heim_shader_get_file_size(const char* file_path) {
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        HEIM_LOG_ERROR("Failed to open file %s", file_path);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    uint64_t file_size = ftell(fp);
    fclose(fp);
    return file_size;
}

bool heim_shader_get_source(const char* file_path, char* buffer, uint64_t file_size) {
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        HEIM_LOG_ERROR("Failed to open file %s", file_path);
        return false;
    }

    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';
    fclose(fp);
    return true;
}

uint32_t heim_shader_get_program(const char* vert_source_path, const char* frag_source_path) {
    uint64_t vert_shader_size = heim_shader_get_file_size(vert_source_path);
    if (vert_shader_size == (uint32_t)-1) {
        return -1;
    }
    char* vert_shader_source = HEIM_CALLOC(char, vert_shader_size + 1, HEIM_MEMORY_TYPE_RENDERER);
    bool source_loaded = heim_shader_get_source(vert_source_path, vert_shader_source, vert_shader_size);
    if (!source_loaded) {
        return -1;
    }

    uint64_t frag_shader_size = heim_shader_get_file_size(frag_source_path);
    if (frag_shader_size == (uint32_t)-1) {
        return -1;
    }
    char* frag_shader_source = HEIM_CALLOC(char, frag_shader_size + 1, HEIM_MEMORY_TYPE_RENDERER);
    source_loaded = heim_shader_get_source(frag_source_path, frag_shader_source, frag_shader_size);
    if (!source_loaded) {
        return -1;
    }

    int success;
    char infoLog[512];

    unsigned int vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const GLchar**)&vert_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        HEIM_LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const GLchar**)&frag_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        HEIM_LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        HEIM_LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    HEIM_FREE(vert_shader_source, HEIM_MEMORY_TYPE_RENDERER);
    HEIM_FREE(frag_shader_source, HEIM_MEMORY_TYPE_RENDERER);

    return shader_program;
}

void heim_shader_set_uniform1i(HeimShader* shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->program, name), value);
}

void heim_shader_set_uniform1f(HeimShader* shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->program, name), value);
}

void heim_shader_set_uniform2f(HeimShader* shader, const char* name, HeimVec2f value) {
    glUniform2f(glGetUniformLocation(shader->program, name), value.x, value.y);
}

void heim_shader_set_uniform3f(HeimShader* shader, const char* name, HeimVec3f value) {
    glUniform3f(glGetUniformLocation(shader->program, name), value.x, value.y, value.z);
}

void heim_shader_set_uniform4f(HeimShader* shader, const char* name, HeimVec4f value) {
    glUniform4f(glGetUniformLocation(shader->program, name), value.x, value.y, value.z, value.w);
}

void heim_shader_set_uniform_mat4(HeimShader* shader, const char* name, HeimMat4 value) {
    float m[16] = {
        value.m[0][0],
        value.m[0][1],
        value.m[0][2],
        value.m[0][3],
        value.m[1][0],
        value.m[1][1],
        value.m[1][2],
        value.m[1][3],
        value.m[2][0],
        value.m[2][1],
        value.m[2][2],
        value.m[2][3],
        value.m[3][0],
        value.m[3][1],
        value.m[3][2],
        value.m[3][3],
    };

    glUniformMatrix4fv(glGetUniformLocation(shader->program, name), 1, GL_FALSE, m);
}

void heim_shader_set_uniform_mat3(HeimShader* shader, const char* name, HeimMat3 value) {
    float m[9] = {
        value.m[0][0],
        value.m[0][1],
        value.m[0][2],
        value.m[1][0],
        value.m[1][1],
        value.m[1][2],
        value.m[2][0],
        value.m[2][1],
        value.m[2][2],
    };

    glUniformMatrix3fv(glGetUniformLocation(shader->program, name), 1, GL_FALSE, m);
}