#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
    //vec3 lighting = vec3(1.0); // placeholder for lighting calculation
    //vec4 texColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(lighting, 1.0) * texColor;
    FragColor = vec4(0.76, 0.70, 0.33, 1.0);
}
