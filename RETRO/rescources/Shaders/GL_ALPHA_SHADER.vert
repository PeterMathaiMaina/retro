#version 330 core

layout (location = 0) in vec3 aPos;  
layout (location = 1 ) in vec2 aTexCoords;

out vec4 FragColor;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    gl_Position = u_Projection * u_View * worldPos;
}