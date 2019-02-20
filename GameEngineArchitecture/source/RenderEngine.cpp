#include "RenderEngine.h"
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

void RenderEngine::InitShaders()
{
	m_DefaultShader =  std::make_shared<ShaderProgram>();
	m_DefaultShader->CompileShader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag");
	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/arial.ttf", s_m_ScreenWidth, s_m_ScreenHeight, m_DefaultShader->GetID());
	m_FontRenderer->SetShader("resources/shaders/fontShader.vert", "resources/shaders/fontShader.frag");

	m_CurrentShader = m_DefaultShader;
	SetDefaultShader();
}

RenderEngine::RenderEngine(int p_ScreenWidth, int p_ScreenHeight)
{
	s_m_ScreenWidth = p_ScreenWidth;
	s_m_ScreenHeight = p_ScreenHeight;

	InitShaders();
}

void RenderEngine::DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 & p_ModelMatrix)
{
	m_CurrentShader->SetMat4("model", p_ModelMatrix);
	p_Model->Render(m_CurrentShader->GetID());
}

void RenderEngine::Update(double p_DeltaTime)
{
	m_DefaultShader->ErrorChecker();
	SetLightParams();
	SetShaderParams();
}

void RenderEngine::SetCamera(std::shared_ptr<CameraComponent> p_Camera)
{
	m_Camera = p_Camera;
}

void RenderEngine::SetGameObjects(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> p_GameObjects)
{
	m_GameObjects = p_GameObjects;
}

void RenderEngine::SetDefaultShader()
{
	glUseProgram(m_DefaultShader->GetID());
}

void RenderEngine::SetCurrentShader(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	m_CurrentShader = p_ShaderProgram;
}

void RenderEngine::SetLightParams()
{
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera->m_FieldOfView), (float)s_m_ScreenWidth / (float)s_m_ScreenHeight, 0.1f, 100.0f);

	m_CurrentShader->SetMat4("projection", projection);
	m_CurrentShader->SetMat4("view", m_Camera->GetViewMatrix());

	// Be sure to activate shader when setting uniforms/drawing objects.
	m_CurrentShader->SetVec3("objectColour", 1.0f, 0.6f, 0.61f);
	m_CurrentShader->SetVec3("lightColour", 1.0f, 1.0f, 1.0f);
	m_CurrentShader->SetVec3("lightPos", 0.0f, 2.0f, -2.0f);
	m_CurrentShader->SetVec3("viewPos", m_Camera->Position());
}

void RenderEngine::SetShaderParams()
{

}
