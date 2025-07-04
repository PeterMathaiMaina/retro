#version 330 core

layout (location = 0) in vec3 aPos;        // Vertex Position
layout (location = 1) in vec3 aNormal;     // Vertex Normal
layout (location = 2) in vec2 aTexCoords;  // Texture Coordinates
layout (location = 3) in vec3 aTangent;    // Tangent
layout (location = 4) in vec3 aBitangent;  // Bitangent
layout (location = 5) in ivec4 aBoneIDs;   // Bone IDs (integer input)
layout (location = 6) in vec4 aWeights;    // Bone Weights

// Optional outputs for fragment shader (depending on your pipeline)
out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;


void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPosLightSpace = lightSpaceMatrix * worldPos;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // Normal in world space
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
