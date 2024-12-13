#version 330 core
in vec2 texCoords;
in vec4 fragColor;
out vec4 FragColor;
uniform sampler2D particleTexture;

void main() {

    vec4 texColor = texture(particleTexture, texCoords);
    FragColor = fragColor;
}