#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 tex_coords;

uniform mat4 projection, view, model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.f);
    tex_coords = aTexCoord;
}