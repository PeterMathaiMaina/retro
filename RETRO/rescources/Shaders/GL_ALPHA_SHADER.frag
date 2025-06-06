#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_DiffuseTexture;

void main() {
    FragColor = vec4(0.3, 0.0, 0.0, 0.3); 
}
