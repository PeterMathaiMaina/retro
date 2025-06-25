#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

// mat4 = 4 vec4s in consecutive locations
layout (location = 3) in mat4 instanceModel;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    FragPos = vec3(instanceModel * vec4(aPos, 1.0));

    // Proper normal transformation with normal matrix from model
    Normal = mat3(transpose(inverse(instanceModel))) * aNormal;

    TexCoords = aTexCoords;

    gl_Position = u_Projection* u_View * vec4(FragPos, 1.0);
}
