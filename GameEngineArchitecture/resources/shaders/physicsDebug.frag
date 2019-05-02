
#version 430 core

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

out vec4 FragColour;

void main()
{
    
	FragColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
} 