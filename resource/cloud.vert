#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 projection, view, model;

out vec2 tex_coords;

void main()
{
    vec3 position = vec3(model[3]);

    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up = vec3(view[0][1], view[1][1], view[2][1]);

    mat4 billboardMatrix = mat4(
        vec4(right, 0.f) * 25.f,
        vec4(up, 0.f) * 25.f,
        vec4(cross(up, right), 0.f) * 25.f,
        vec4(position, 1.f)
    );

    gl_Position = projection * view * billboardMatrix * vec4(aPos, 1.f);
    tex_coords = aTex;
}