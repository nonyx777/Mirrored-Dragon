#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 tex_coords;

void main()
{
    gl_Position = vec4(aPos, 0.f, 1.f);
    tex_coords = aTex;
}