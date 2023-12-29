#version 460 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = texCoord;
    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
