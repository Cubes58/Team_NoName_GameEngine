#pragma once
#include "ImageRenderer.h"
#include "ShaderProgram.h"



class ContrastChanger
{
private:
	ImageRenderer *m_Renderer;
	


public:
	ShaderProgram m_Shader;
	ContrastChanger();
	

	void Render(int p_Texture);

	
};