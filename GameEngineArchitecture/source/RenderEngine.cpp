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

	m_Skybox = std::make_shared<Skybox>(150);
	SetLightParams(m_Skybox->GetShaderProgram());
	SetLightParams(m_DefaultShader);
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
	m_DefaultShader->SetMat4("model", p_ModelMatrix);
	p_Model->Render(m_DefaultShader->GetID());
}

void RenderEngine::Update(double p_DeltaTime)
{
	m_Skybox->GetShaderProgram()->ErrorChecker();
	SetShaderParams(m_Skybox->GetShaderProgram());

	m_DefaultShader->ErrorChecker();
	SetShaderParams(m_DefaultShader);
}

void RenderEngine::Render()
{
	m_Skybox->Render();
	SetDefaultShader();
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

void RenderEngine::SetLightParams(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	glUseProgram(p_ShaderProgram->GetID());
	// Be sure to activate shader when setting uniforms/drawing objects.
	p_ShaderProgram->SetVec3("objectColour", 1.0f, 0.6f, 0.61f);
	p_ShaderProgram->SetVec3("lightColour", 1.0f, 1.0f, 1.0f);
	p_ShaderProgram->SetVec3("lightPos", 0.0f, 2.0f, -2.0f);
}

void RenderEngine::SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	glUseProgram(p_ShaderProgram->GetID());
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera->m_FieldOfView), (float)s_m_ScreenWidth / (float)s_m_ScreenHeight, 0.1f, 300.0f);

	p_ShaderProgram->ErrorChecker();
	p_ShaderProgram->SetMat4("projection", projection);
	p_ShaderProgram->SetMat4("view", m_Camera->GetViewMatrix());
	p_ShaderProgram->SetMat4("viewSky", glm::mat4(glm::mat3(m_Camera->GetViewMatrix())));
	p_ShaderProgram->SetVec3("viewPos", m_Camera->Position());
	p_ShaderProgram->ErrorChecker();

}

void RenderEngine::RenderText(const std::string & p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour)
{
	m_FontRenderer->RenderText(p_Text, p_XPosition, p_YPosition, p_Scale, p_Colour);
}
