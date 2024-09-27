#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord; 


uniform sampler2D uTexture; 
uniform sampler2D uTexture2;
uniform float uTime;

void main()
{
	vec4 texColor = texture(uTexture, TexCoord);
	vec4 texColor2 = texture(uTexture2, TexCoord);
	FragColor = texColor2 + (texColor);
};
