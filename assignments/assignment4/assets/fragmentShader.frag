
#version 330 core
out vec4 FragColor;
in vec4 ourColor;

in vec2 texCoord;

uniform float uTime;
uniform sampler2D ourTexture;

void main()
{
	vec4 texColor = texture(ourTexture, texCoord);

	FragColor = texColor;
};

