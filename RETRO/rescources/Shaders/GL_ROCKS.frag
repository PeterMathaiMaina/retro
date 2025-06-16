#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse;

void main()
{
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor   = vec4(0.5,0.0,0.0,1.0);
}
