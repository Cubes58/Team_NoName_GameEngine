#include "ContrastChanger.h"

ContrastChanger::ContrastChanger()
{
	m_Shader.CompileShader("resources/shaders/contrastShader.vert", "resources/shaders/contrastShader.frag");
}

void ContrastChanger::Render(int p_Texture)
{
	glUseProgram(m_Shader.GetID());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, p_Texture);
	m_Shader.SetInt("colourTexture", 0);
}


