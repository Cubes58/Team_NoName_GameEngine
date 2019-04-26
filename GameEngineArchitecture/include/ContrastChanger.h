#pragma once
#include "ImageRenderer.h"
#include "VBOQuad.h"
#include "ShaderProgram.h"



class ContrastChanger
{
private:
	VBOQuad* m_screenQuad;
	


public:
	ShaderProgram m_Shader;
	ContrastChanger();
	

	void Render(int p_Texture);

	
};