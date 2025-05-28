#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_DiffuseTexture;

void main() {
    vec4 texColor = texture(u_DiffuseTexture, TexCoords);

    // Optional: Discard low alpha fragments (for hard edges)
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
