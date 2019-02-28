#version 430 core

out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D colourTexture;
uniform float nearPlane;
uniform float farPlane;

// Required when using perspective projection matrix.
float LinearizeDepth(float p_Depth) {
	float z = p_Depth * 2.0f - 1.0f;	// Back to NDC.
	return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main() {
	float depthValue = texture(colourTexture, TexCoords).r;
	// FragColour = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0f);	// Perspective.
	FragColour = vec4(vec3(depthValue), 1.0f);	// Orthographic.
}