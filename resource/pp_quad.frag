#version 330 core

uniform sampler2D screenTexture;

out vec4 fragColor;
in vec2 tex_coords;

void main()
{
    fragColor = texture(screenTexture, tex_coords);
}
