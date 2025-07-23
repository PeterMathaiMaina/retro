#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D terrain_texture;

void main() {
    FragColor =texture(terrain_texture,TexCoords)*0.5; 
    // FragColor = vec4(0.0, 0.3, 0.0, 1.0); // basic green terrain
}
