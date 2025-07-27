#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;

void main() {
    vec3 hdrColor = vec3(texture(hdrBuffer, TexCoords));

    // Tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / 2.2));

    FragColor = vec4(mapped, 1.0);
    // FragColor = texture(hdrBuffer,TexCoords);
}
