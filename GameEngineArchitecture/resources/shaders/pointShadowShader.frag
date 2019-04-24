#version 430 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColour;

uniform float far_plane;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 p_fragPos)
{
	vec3 fragToLight = p_fragPos - lightPos;
	float currentDepth = length(fragToLight);
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - p_fragPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
	
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= far_plane;
		if(currentDepth - bias > closestDepth)
		{
			shadow += 1.0;
		}
	}
	shadow /= float(samples);
	return shadow;

}

void main()
{
	vec3 colour = texture(texture_diffuse1, TexCoords).rgb;
	vec3 normal = normalize(Normal);
	
	vec3 ambient = 0.3 * colour;
	
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColour;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColour;
	
	float shadow = ShadowCalculation(FragPos);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * colour;
	
	FragColor = vec4(lighting, 1.0);
}