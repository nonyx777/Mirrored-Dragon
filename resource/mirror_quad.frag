#version 330 core

uniform sampler2D mirrorTexture;

out vec4 fragColor;
in vec2 tex_coords;

void main()
{
    fragColor = texture(mirrorTexture, tex_coords);
    //fragColor = vec4(1.f, 1.f, 1.f, 1.f);
}