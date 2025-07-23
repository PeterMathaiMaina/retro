#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D terrain_texture;

void main() {
    // FragColor =texture(terrain_texture,TexCoords)*0.5; 
    FragColor = vec4(vec3(0.73/2, 0.64/2, 0.38/2), 1.0); // basic green terrain
}
