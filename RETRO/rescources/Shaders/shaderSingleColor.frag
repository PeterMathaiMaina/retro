#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

void main()
{
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}
