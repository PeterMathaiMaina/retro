#version 330 core
layout (location = 0) in vec3 aPos;         // Vertex positions
layout (location = 1) in vec3 aNormal;      // Vertex normals
layout (location = 2) in vec2 aTexCoords;   // Vertex texture coordinates
layout (location = 3) in vec3 aTangent;     // Vertex tangent
layout (location = 4) in vec3 aBitangent;   // Vertex bitangent

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
//out mat3 TBN; // Tangent-Bitangent-Normal matrix (for normal mapping)

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_scaleMatrix;

void main()
{
    
    // Calculate the final vertex position
    vec4 worldPos = u_Model * vec4(aPos, 1.0);//u_scaleMatrix
    gl_Position = u_Projection * u_View * worldPos;

    // Pass relevant data to the fragment shader
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(u_Model))) * aNormal; // Transform normals to world space
    TexCoords = aTexCoords;
}
