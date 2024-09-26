
#version 330 core
out vec4 FragColor;
in vec4 ourColor;

uniform float uTime;

void main()
{
	FragColor = ourColor;
};

