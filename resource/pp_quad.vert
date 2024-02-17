#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 tex_coords;

void main()
{
    gl_Position = vec4(aPos, 1.f);
    tex_coords = aTex;
}