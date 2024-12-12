#version 330 core

out vec4 FragColor;

in vec3 TexCoords; 

in vec3 textureDir; // direction vector representing a 3D texture coordinate 
uniform samplerCube skybox; 

void main()
{
	FragColor = texture(skybox, TexCoords);
}