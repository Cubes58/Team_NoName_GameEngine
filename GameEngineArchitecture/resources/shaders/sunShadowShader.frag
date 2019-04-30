#version 430 core

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 FragColour;
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColour;

uniform sampler2D texture_diffuse1;
uniform sampler2D depthMap;



float ShadowCalculation(vec4 p_FragPosLightSpace) {

  
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
   
    projCoords = projCoords * 0.5 + 0.5;
   
    float closestDepth = texture(depthMap, projCoords.xy).r; 
   
    float currentDepth = projCoords.z;
   
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main() 
{
	float objectNearestLight = texture(depthMap, FragPosLightSpace.xy).r;
	float lightFactor = 1.0;
	if(FragPosLightSpace.z > objectNearestLight){
		lightFactor = 1.0 - 0.4;
	}
	
	vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColour;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColour;    
    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace);                      
    vec3 lighting = (diffuse * lightFactor) * color + specular;    
    
    FragColour = vec4(lighting, 1.0);
}