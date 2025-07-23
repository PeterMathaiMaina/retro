#version 450
layout (location = 0) in vec3 aPos;      
layout (location = 1) in vec3 aNormal;      
layout (location = 2) in vec2 aTexcoords; 



out vec2 TexCoords;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexcoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = aNormal;
}