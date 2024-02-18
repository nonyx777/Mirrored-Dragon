#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;
layout (location = 2) in vec4 instance1Layout;
layout (location = 3) in vec4 instance2Layout;
layout (location = 4) in vec4 instance3Layout;
layout (location = 5) in vec4 instance4Layout;

out vec2 tex_coords;

uniform mat4 projection, view, model;

mat4 instanceMatrix = mat4(instance1Layout, instance2Layout, instance3Layout, instance4Layout);

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.f);
    tex_coords = aTextureCoord;  
}