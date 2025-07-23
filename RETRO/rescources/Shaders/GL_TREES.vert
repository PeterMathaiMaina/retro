#version 450
layout (location = 0) in vec3 aPos;      
layout (location = 1) in vec2 aTexcoords; 


out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexcoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}