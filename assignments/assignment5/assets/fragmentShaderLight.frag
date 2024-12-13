
#version 330 core
out vec4 FragColor;

uniform vec3 ourColor; 
uniform vec3 lightPos;

in vec3 worldNorm;
in vec3 worldPos;

void main()
{


	FragColor = vec4(ourColor, 1.0f);
	
};