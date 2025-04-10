#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
//in vec3 ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixture;

void main()
{
    // Blend the two textures based on the 'mixture' value
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), mixture);
}
