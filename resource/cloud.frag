#version 330 core

out vec4 fragColor;
uniform sampler2D cloudTexture;
in vec2 tex_coords;

void main()
{
    vec4 texture_color = texture(cloudTexture, tex_coords);
    if(texture_color.a < 0.1)
        discard;
    fragColor = texture_color;
}