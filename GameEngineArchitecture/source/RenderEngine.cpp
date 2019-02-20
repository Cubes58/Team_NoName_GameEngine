#include "..\include\RenderEngine.h"

void RenderEngine::InitShaders()
{
	m_DefaultShader.CompileShader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag");
	m_FontRenderer = new FontRenderer("resources/fonts/arial.ttf", s_m_ScreenWidth, s_m_ScreenHeight, m_DefaultShader.GetID());
	m_FontRenderer->SetShader("resources/shaders/fontShader.vert", "resources/shaders/fontShader.frag");
}
