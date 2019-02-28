#version 430 core

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 FragColour;
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColour;
uniform vec3 objectColour;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 p_FragPosLightSpace) {
	// Perform perspective division.
	vec3 projCoords = p_FragPosLightSpace.xyz / p_FragPosLightSpace.w;
	
	// Transform to [0, 1] range.
	projCoords = projCoords * 0.5 + 0.5;
	
	// Get the closestdepth value from the light's perspective (using [0,1] range p_FragPosLightSpace as coords).
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	
	// Get the depth value of the current fragment, from the light's perspective.
	float currentDepth = projCoords.z;
	
	// Calculate the bias offset (based on the depth map resolution and slope).
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);
	
	// Check whether current frag pos is in a shadow.
	// float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
	
	// PCF
	float shadow = 0.0f;
	
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
		}
	}
	shadow /= 9.0f;
	
	// Keep the shadow at 0.0f, when it's outside the far_plane region of the light's frustum.
	if (projCoords.z > 1.0f) {
		shadow = 0.0f;
	}

	return shadow;
}

void main() {
	vec3 colour = texture(texture_diffuse1, TexCoords).rgb;
	vec3 normal = normalize(Normal);
	
	// Ambient.
	vec3 ambient = 0.3f * colour;
	
	// Diffuse.
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColour;
	
	// Specular.
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0f;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
	vec3 specular = spec * lightColour;
	
	// Calculate shadow.
	float shadow = ShadowCalculation(FragPosLightSpace);
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * colour;
	
	FragColour = vec4(lighting, 1.0f);
}
