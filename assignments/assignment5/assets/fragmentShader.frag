
#version 330 core
out vec4 FragColor;
in vec4 ourColor;

in vec2 texCoord;

in vec3 Normal;
in vec3 FragPos;

uniform float uTime;
uniform float specularStrength;
uniform float ambientStrength;
uniform float diffStrength;
uniform float shininess;
uniform sampler2D ourTexture;
uniform float rimThreshold; 
uniform float rimcut; 
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{

	vec4 texColor = texture(ourTexture, texCoord);
	if(texColor.a < 0.1)
		discard;
	
	vec3 ambient = ambientStrength * lightColor;
	

	vec3 norm = normalize(Normal); 
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
    
	//float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos); 
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 1025 - shininess);
	
	vec3 specular = specularStrength * lightColor * spec; 
	vec3 result = (ambient + diffuse + specular ) * texColor.rgb;
	
	 //vec3 result = lighting * texColor.rgb * objectColor; 
	
	FragColor = vec4(result, 1.0);

}
