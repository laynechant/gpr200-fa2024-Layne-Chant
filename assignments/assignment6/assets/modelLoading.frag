#version 330 core
out vec4 FragColor; 

in vec2 TexCoords; 
in vec4 ourColor;

in vec2 texCoord;

in vec3 Normal;
in vec3 FragPos;

uniform float uTime;
uniform float specularStrength;
uniform float ambientStrength;
uniform float diffStrength;
uniform float shininess;
uniform float rimThreshold; 
uniform float rimcut; 


uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1; 

void main()
{

vec3 ambient = ambientStrength * lightColor;

vec3 norm = normalize(Normal); 
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 1.0);
vec3 diffuse = diff * lightColor;
float intensity = dot(lightDir,normalize(Normal));
   vec4 color; 
   
   if (intensity > 0.95)
        color = vec4(0.0,0.0,0.0,1.0);
    else if (intensity > 0.75)
        color = vec4(0.0,0.0,0.0,1.0);
    else if (intensity > 0.5)
        color = vec4 (0.0,0.0,0.0,1.0);
    else
        color = vec4(1.0,1.0,1.0,1.0);

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos); 
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, norm);

	
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 1025 - shininess);
	vec3 specular = specularStrength * lightColor * spec; 

	vec3 result = (ambient + diffuse + specular) * FragColor.rgb;
   float rimLight = 1.0 - max(dot(normalize(viewPos - FragPos), norm), 0.0);
	float rimIntensity = pow(rimLight,rimThreshold);
	rimIntensity = smoothstep(rimcut - 0.01, rimcut + 0.01, rimIntensity);
	result += rimIntensity; 
    float rimShadow = smoothstep(0.0,1.0,(intensity * 0.5) / rimcut + 0.5);
	result = mix(vec3(0.2) * FragColor.rgb, result * FragColor.rgb, rimShadow);
	FragColor = vec4(result * FragColor.rgb, 1.0);

	vec4 result4 = vec4(result.x, result.y, result.z, 1.0);
	//vec4 result4 = vec4(1.0, 0.0, 1.0, 1.0);
	FragColor =  texture(texture_diffuse1 , TexCoords); //result4 
}
