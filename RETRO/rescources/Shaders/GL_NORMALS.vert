#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out DATA {
    vec3 FragPos;
    vec3 Normal;
} gs_out;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    gs_out.FragPos = worldPos.xyz;
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    gs_out.Normal = normalMatrix * aNormal;

    //gs_out.Normal = mat3(transpose(inverse(u_Model))) * aNormal;

    gl_Position = u_Projection * u_View * worldPos;
}
