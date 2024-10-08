
#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec4 aColor; 
layout (location = 2) in vec2 aTexCoord;

uniform float uTime;

out vec4 ourColor; 
out vec2 TexCoord;

void main()
{
	vec3 pos = aPos;
	
	gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
	ourColor = aColor; // set ourColor to the input color we got from the vertex data
	TexCoord = aTexCoord; 
  };