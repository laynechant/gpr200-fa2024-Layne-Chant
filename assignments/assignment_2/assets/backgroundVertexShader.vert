#version 330 core
layout(location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord; // the color variable has attribute position 1

uniform float uTime;

out vec4 ourColor; // output a color to the fragment shader
out vec2 TexCoord;

void main()
{
	vec3 pos = aPos;
	//pos.y += abs(sin((uTime * 2)) * 0.1);
	gl_Position = vec4(pos.x * 4, (pos.y * 4) * (2 + sin(uTime)), pos.z, 1.0); // see how we directly give a vec3 to vec4's constructor
	//ourColor = aColor; // set ourColor to the input color we got from the vertex data
	vec2 newTexCoord = aTexCoord;
	newTexCoord.x += uTime / 2;
	newTexCoord.y += sin(uTime / 2);
	TexCoord = newTexCoord * 10;
};