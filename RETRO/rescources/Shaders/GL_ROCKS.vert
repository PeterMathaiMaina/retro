#version 330 core

layout (location = 0) in vec3 aPos;         // vertex position
layout (location = 1) in vec3 aNormal;      // normal (optional)
layout (location = 2) in vec2 aTexCoords;   // texture coords (optional)
layout (location = 3) in mat4 instanceModel; // model matrix for instancing (mat4 uses locations 7-10)

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;       // to fragment shader
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(instanceModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(instanceModel))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0);
}
