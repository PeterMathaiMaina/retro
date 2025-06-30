#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 instanceModel;

uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    vec4 worldPosition = instanceModel * vec4(aPos, 1.0);
    FragPos = worldPosition.xyz;
    Normal = mat3(transpose(inverse(mat3(instanceModel)))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = lightSpaceMatrix * worldPosition;
}
