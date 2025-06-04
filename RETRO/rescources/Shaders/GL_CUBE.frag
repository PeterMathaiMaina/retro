#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_DiffuseTexture;

void main() {
    vec4 texColor = texture(u_DiffuseTexture, vec2(TexCoords.x, 1.0 - TexCoords.y));

    FragColor = texColor; 
}
