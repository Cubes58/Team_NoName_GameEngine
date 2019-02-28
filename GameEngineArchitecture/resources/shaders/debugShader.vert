
#version 440 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 texture_coordinates;

out vec2 TexCoords;

void main() {
	TexCoords = texture_coordinates;
	gl_Position = vec4(vertex_position, 1.0f);
}