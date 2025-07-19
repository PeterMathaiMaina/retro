#version 450 core

// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoords;
// layout (location = 4) in vec3 tangent;

// out vec2 texCoords;
// out vec3 TangentLightDir;
// out vec3 TangentViewDir;
// out vec3 TangentFragPos;
// out vec3 Normal; 

// uniform mat4 view;
// uniform mat4 projection;
// uniform vec3 viewPos;
// uniform vec3 aSpotlightPositon;
// uniform mat4 model;

void main1()
{
    vec3 FragPosWorld = vec3(model * vec4(aPos, 1.0));
    vec3 T = normalize(mat3(model) * tangent);
    vec3 N = normalize(mat3(transpose(inverse(model))) * aNormal); 
    vec3 B = normalize(cross(N, T));

    mat3 TBN = transpose(mat3(T, B, N));

    texCoords = aTexCoords;

    TangentLightDir = TBN * (aSpotlightPositon - FragPosWorld);
    TangentViewDir = TBN * (viewPos - FragPosWorld);
    TangentFragPos = TBN * FragPosWorld;


    Normal = aNormal;   

    gl_Position = projection * view * vec4(FragPosWorld, 1.0);
}

layout(location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout (location = 4) in vec3 tangent;


out vec2 texCoords;
out vec3 TangentLightDir;
out vec3 TangentViewDir;
out vec3 TangentFragPos;
out vec3 Normal; 

uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform vec3 aSpotlightPositon;
uniform mat4 model;

void main() {
    vec3 FragPosWorld = vec3(model * vec4(aPos, 1.0));
    vec3 T = normalize(mat3(model) * tangent);
    vec3 N = normalize(mat3(transpose(inverse(model))) * aNormal); 
    vec3 B = normalize(cross(N, T));

    mat3 TBN = transpose(mat3(T, B, N));

    texCoords = aTexCoords;

    TangentLightDir = TBN * (aSpotlightPositon - FragPosWorld);
    TangentViewDir = TBN * (viewPos - FragPosWorld);
    TangentFragPos = TBN * FragPosWorld;


    Normal = aNormal;   

    gl_Position = projection * view * vec4(FragPosWorld, 1.0);
}
