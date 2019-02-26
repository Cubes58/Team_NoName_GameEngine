#version 430 core

in vec2 textureCoords;

out vec4 out_Colour;

uniform sampler2D colourTexture;

void main()
{
	out_Colour = texture(colourTexture, textureCoords).rbga;
}