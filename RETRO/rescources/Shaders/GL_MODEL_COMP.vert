
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Uniforms
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Output block to geometry shader
//out DATA {
//    vec3 Normal;
//    vec2 TexCoords;
//    vec3 FragPos;
//} data_out;
out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

void main()
{
    vec4 worldPosition = u_Model * vec4(aPos, 1.0);
    gl_Position = u_Projection * u_View * worldPosition;

    FragPos = vec3(worldPosition);
    Normal = mat3(transpose(inverse(u_Model))) * aNormal; 
    TexCoords = aTexCoords;
}

