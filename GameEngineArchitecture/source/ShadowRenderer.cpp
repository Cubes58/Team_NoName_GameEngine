#include "ShadowRenderer.h"

#include "ShadowBox.h"
#include "ShaderProgram.h"
#include "FrameBufferObject.h"
#include "CameraComponent.h"
#include "RenderEngine.h"




ShadowRenderer::ShadowRenderer(int p_ShadowSize, std::shared_ptr<CameraComponent> p_SceneCamera, RenderEngine * p_RenderEngine)
{
	m_ProjectionMatrix = glm::mat4();
	m_LightViewMatrix = glm::mat4();
	m_ProjViewMatrix = glm::mat4();
	m_OffsetMatrix = glm::mat4();

	m_ShadowShader = new ShaderProgram();
	m_ShadowShader->CompileShader("resources/shaders/shadowShaderDepth.vert", "resources/shaders/shadowShaderDepth.frag");

	m_ShadowBox = new ShadowBox(p_SceneCamera, m_LightViewMatrix);
	m_FrameBuffer = new FrameBufferObject(p_ShadowSize, p_ShadowSize, FrameBufferType::SHADOW_BUFFER);
}


void ShadowRenderer::Prepare(glm::vec3 p_SunPosition)
{
	UpdateOrthoProjectionMatrix(m_ShadowBox->GetWidth(), m_ShadowBox->GetHeight(), m_ShadowBox->GetLength());
	UpdateLightViewMatrix(p_SunPosition, m_ShadowBox->GetCenter());
	m_ProjViewMatrix = m_ProjectionMatrix * m_LightViewMatrix;
	SetShaderParams();

}

void ShadowRenderer::UpdateLightViewMatrix(glm::vec3 p_Direction, glm::vec3 p_Center)
{
	p_Direction = glm::normalize(p_Direction);
	p_Center = glm::vec3(-p_Center.x, -p_Center.y, -p_Center.z);
	m_LightViewMatrix = glm::mat4();
	float l_Pitch = (float)glm::acos(glm::vec2(p_Direction.x, p_Direction.y).length());
	m_LightViewMatrix = glm::rotate(m_LightViewMatrix, l_Pitch, glm::vec3(1, 0, 0));
	float l_Yaw = (float)glm::degrees((float)glm::atan(p_Direction.x / p_Direction.z));
	l_Yaw = p_Direction.z > 0 ? l_Yaw - 180 : l_Yaw;
	m_LightViewMatrix = glm::rotate(m_LightViewMatrix, -glm::radians(l_Yaw), glm::vec3(0, 1, 0));

	m_LightViewMatrix = glm::translate(m_LightViewMatrix, p_Center);
}

void ShadowRenderer::UpdateOrthoProjectionMatrix(float p_Width, float p_Height, float p_Length)
{
	m_ProjectionMatrix = glm::mat4();
	m_ProjectionMatrix[0][0] = 2.f / p_Width;
	m_ProjectionMatrix[1][1] = 2.f / p_Height;
	m_ProjectionMatrix[2][2] = -2.f / p_Length;
	m_ProjectionMatrix[3][3] = 1;
}

void ShadowRenderer::CreateOffsetMatrix()
{
	m_OffsetMatrix = glm::translate(m_OffsetMatrix, glm::vec3(0.5f));
	m_OffsetMatrix = glm::scale(m_OffsetMatrix, glm::vec3(0.5f));
}

int ShadowRenderer::GetShadowMap()
{
	return m_FrameBuffer->GetDepthTexture();
}

glm::mat4 ShadowRenderer::GetLightViewMatrix()
{
	return m_LightViewMatrix;
}

glm::mat4 ShadowRenderer::GetShadowSpaceMatrix()
{
	return m_OffsetMatrix * m_ProjectionMatrix;
}

void ShadowRenderer::Render(glm::vec3 p_SunPosition)
{
	m_ShadowBox->Update();
	Prepare(p_SunPosition);
	m_FrameBuffer->BindFrameBuffer();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_RenderEngine->RenderSceneObjects(std::make_shared<ShaderProgram>(m_ShadowShader));
	m_FrameBuffer->UnbindFrameBuffer();
}

void ShadowRenderer::SetShaderParams()
{
	m_RenderEngine->SetLightParams(std::make_shared<ShaderProgram>(m_ShadowShader));
	m_RenderEngine->SetShaderParams(std::make_shared<ShaderProgram>(m_ShadowShader));
}
