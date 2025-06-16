#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;

void main() {
    //vec4 texColor = texture(texture_diffuse, vec2(TexCoords.x, 1.0 - TexCoords.y));
    //FragColor = vec4(texColor);
    FragColor = vec4(0.7f, 0.0f, 0.0f,0.8f);
//    FragColor = texColor; 
}
