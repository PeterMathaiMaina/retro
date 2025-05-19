#version 330 core
layout (location = 0) in vec3 aPos;         // Vertex position
layout (location = 1) in vec3 aNormal;      // Vertex normal
layout (location = 2) in vec2 aTexCoords;   // Texture coordinates
layout (location = 3) in vec3 aTangent;     // Tangent (not used yet)
layout (location = 4) in vec3 aBitangent;   // Bitangent (not used yet)

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = u_Projection * u_View * worldPos;
}
