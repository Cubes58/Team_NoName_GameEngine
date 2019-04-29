#include <Shadows.h>

Shadows::Shadows(float p_Width, float p_Height, bool p_IsDirectional)
{
	if (p_IsDirectional)
	{
		m_ShadowBuffer = std::make_shared<FrameBufferObject>(p_Width, p_Height, FrameBufferType::DIRECTIONAL_SHADOW);
		m_ShadowDepthShader = std::make_shared<ShaderProgram>();
		m_ShadowDepthShader->CompileShader("resources/shaders/directionalDepth.vert", "resources/shaders/directionalDepth.frag");
	}
	else
	{
		m_ShadowBuffer = std::make_shared<FrameBufferObject>(p_Width, p_Height, FrameBufferType::POINT_SHADOW);
		m_ShadowDepthShader = std::make_shared<ShaderProgram>();
		m_ShadowDepthShader->CompileShader("resources/shaders/pointDepth.vert", "resources/shaders/pointDepth.frag", "resources/shaders/pointDepth.geo");
	}
	m_Width = p_Width;
	m_Height = p_Height;
	m_IsDirectional = p_IsDirectional;
}

void Shadows::Prepare(glm::vec3 p_SunPosition)
{
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_Width / (float)m_Height, 0.1f, 200.f);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(p_SunPosition, p_SunPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(p_SunPosition, p_SunPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(p_SunPosition, p_SunPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(p_SunPosition, p_SunPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(p_SunPosition, p_SunPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(p_SunPosition, p_SunPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	m_ShadowDepthShader->Use();
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_ShadowDepthShader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}

	m_ShadowBuffer->BindFrameBuffer();
}

std::shared_ptr<ShaderProgram> Shadows::Render()
{
	return m_ShadowDepthShader;
}

void Shadows::End(unsigned int p_Width, unsigned int p_Height)
{
	m_ShadowBuffer->UnbindFrameBuffer(p_Width, p_Height);
}

unsigned int Shadows::GetShadowMap()
{
	if (m_IsDirectional)
	{
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, m_ShadowBuffer->GetDepthTexture());
	}
	else
	{
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowBuffer->GetDepthTexture());
	}

	return 10;
}
