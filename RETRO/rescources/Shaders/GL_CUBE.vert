#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;              // ✅ vec3, not vec2
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    vec4 worldPosition = instanceMatrix * vec4(aPos, 1.0);

    Normal = mat3(transpose(inverse(instanceMatrix))) * aNormal; // correct transformation

    FragPos = vec3(worldPosition);
    TexCoords = aTexCoords;
    gl_Position = u_Projection * u_View * worldPosition;
}
