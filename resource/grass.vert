#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;
layout (location = 2) in vec4 instance1Layout;
layout (location = 3) in vec4 instance2Layout;
layout (location = 4) in vec4 instance3Layout;
layout (location = 5) in vec4 instance4Layout;
layout (location = 6) in float heightLayout;

out vec2 tex_coords;

uniform mat4 projection, view;
uniform float rotation_angle;

mat4 instanceMatrix = mat4(instance1Layout, instance2Layout, instance3Layout, instance4Layout);

float grass_height = heightLayout;

vec3 rotateVector(vec3 v, float angle, vec3 axis) {
    float s = sin(angle);
    float c = cos(angle);
    float oneMinusC = 1.0 - c;

    vec3 rotatedVector = v * c + cross(axis, v) * s + axis * dot(axis, v) * oneMinusC;
    return rotatedVector;
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


void main()
{
    //billboarding
    mat4 model = instanceMatrix;
    
    // Extract position from the model matrix
    vec3 position = vec3(model[3]);

    // Calculate the camera's right and up vectors in world space
    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up = vec3(view[0][1], view[1][1], view[2][1]);

    // Calculate the billboard matrix
    mat4 billboardMatrix = mat4(
        vec4(right, 0.0) * grass_height,
        vec4(up, 0.0) * grass_height,
        vec4(cross(up, right), 0.0) * grass_height,
        vec4(position, 1.0)
    );

    float random = rand(aTextureCoord);
    //rotation stuff
    vec3 rotationVector = rotateVector(aPos, rotation_angle * random, vec3(0.f, 0.f, 1.f));

    gl_Position = projection * view * billboardMatrix * vec4(rotationVector, 1.f);
    tex_coords = aTextureCoord;  
}