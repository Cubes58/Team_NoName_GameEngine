#include "..\include\PostProcessor.h"

void PostProcessor::Start()
{
	glBindVertexArray(m_ScreenQuad.GetVaoHandle());
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

void PostProcessor::End()
{
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void PostProcessor::InitPostProcessing()
{
	m_ContrastChanger = new ContrastChanger();
}

void PostProcessor::DoPostProcessing(int p_ColourTexture)
{
	Start();
	m_ContrastChanger->Render(p_ColourTexture);
	End();
}
