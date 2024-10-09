
#version 330 core
layout(location = 0) in vec3 aPos; 
layout (location = 1) in vec4 aColor; 

uniform float uTime;

out vec4 ourColor; 

void main()
{
	vec3 pos = aPos;
	pos.y += sin((uTime * 4) + pos.x) * 0.2;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0); // see how we directly give a vec3 to vec4's constructor
	ourColor = aColor; // set ourColor to the input color we got from the vertex data
};