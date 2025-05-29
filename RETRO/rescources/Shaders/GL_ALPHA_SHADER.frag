#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_DiffuseTexture;

void main() {
    vec4 texColor = texture(u_DiffuseTexture, vec2(TexCoords.x, 1.0 - TexCoords.y));
    // Optional: Discard low alpha fragments (for hard edges)
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
    //FragColor = vec4(0.9, 0.9, 0.9, 0.8); // Light blue, 30% opaque
}
