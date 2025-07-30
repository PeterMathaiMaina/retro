#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D terrain_texture;

void main() {
    // vec3 result = vec3(0.0);
    // result  = texture(terrain_texture,TexCoords).rgb * 0.2;
    // FragColor = vec4(result,1.0);
    FragColor = vec4(vec3(0, 0.1, 0), 1.0); // basic green terrain
}
