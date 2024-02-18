#version 330 core

out vec4 fragColor;

uniform sampler2D grassTexture;

in vec2 tex_coords;

void main()
{
    vec4 tex_color = texture(grassTexture, tex_coords);
    if(tex_color.a < 0.1f)
        discard;
    
    fragColor = tex_color;
}