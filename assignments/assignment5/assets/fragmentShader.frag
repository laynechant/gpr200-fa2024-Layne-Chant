
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
//uniform float rimThreshold; 
//uniform float rimcut; 
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
	float intensity = dot(lightDir,normalize(Normal));
	//this is cel shading code example that would of been in the house. 
   vec4 color; 
    if (intensity > 0.5)
      color = vec4(1.0,1.0,1.0,1.0);
  else if (intensity > 0)
      color = vec4(0.33,0.33,0.33,1.0);
	vec3 specular = specularStrength * lightColor * spec; 
	//vec3 result = (ambient + diffuse + specular ) * texColor.rgb;

	
	 
	FragColor =  color * texColor;

	
	
//vec3 result = lighting * texColor.rgb * objectColor; 

}
