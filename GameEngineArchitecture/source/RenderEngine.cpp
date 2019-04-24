#include "RenderEngine.h"

#include "ShaderProgram.h"
#include "FontRenderer.h"
#include "SkyBox.h"
#include "FrameBufferObject.h"
#include "PostProcessor.h"
#include "Light.h"

#include "ModelComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

#include "PlayerCharacter.h"
#include "StaticEnvironmentObject.h"
#include "DynamicEnvironmentObject.h"
#include "EnemyTower.h"
#include "EndLevelCollectable.h"


void RenderEngine::Init(int p_ScreenWidth, int p_ScreenHeight)
{
	m_ScreenWidth = p_ScreenWidth;
	m_ScreenHeight = p_ScreenHeight;

	m_Sun = new Light(glm::vec3(1.0f, 50.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	
	m_SceneFrameBuffer = std::make_shared<FrameBufferObject>(p_ScreenWidth, p_ScreenHeight, FrameBufferType::DEPTH_TEXTURE);
	m_ShadowFrameBuffer = std::make_shared<FrameBufferObject>(1024, 1024, FrameBufferType::SHADOW_BUFFER);
	m_PostProcessor = std::make_shared<PostProcessor>();
	m_PostProcessor->InitPostProcessing();
	InitShaders();
}

void RenderEngine::InitShaders()
{
	m_DefaultShader = std::make_shared<ShaderProgram>();
	m_DefaultShader->CompileShader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag");

	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/arial.ttf", m_ScreenWidth, m_ScreenHeight, m_DefaultShader->GetID());
	m_FontRenderer->SetShader("resources/shaders/fontShader.vert", "resources/shaders/fontShader.frag");
	//Setup Shadow Shaders
	m_ShadowShader = std::make_shared<ShaderProgram>();
	m_ShadowShader->CompileShader("resources/shaders/pointShadowShader.vert", "resources/shaders/pointShadowShader.frag");

	m_DepthShader = std::make_shared<ShaderProgram>();
	m_DepthShader->CompileShader("resources/shaders/pointDepth.vert", "resources/shaders/pointDepth.frag", "resources/shaders/pointDepth.geo");

	m_DebugShader = std::make_shared <ShaderProgram>();
	m_DebugShader->CompileShader("resources/shaders/debugShader.vert", "resources/shaders/debugShader.frag");

	
	//Setup Skybox Shaders
	m_Skybox = std::make_shared<Skybox>(150);

	m_DefaultShader->ErrorChecker();
	glEnable(GL_DEPTH_TEST);

	glGenFramebuffers(1, &DepthFBO);
	glGenTextures(1, &DepthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, DepthMap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_ShadowWidth, m_ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_ShadowShader->Use();
	m_ShadowShader->SetInt("depthMap", 1);

	m_DefaultShader->Use();

}

void RenderEngine::DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 & p_ModelMatrix, std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	p_ShaderProgram->ErrorChecker();
	p_ShaderProgram->SetMat4("model", p_ModelMatrix);
	p_ShaderProgram->ErrorChecker();
	p_Model->Render(p_ShaderProgram->GetID());

}

void RenderEngine::Update(double p_DeltaTime)
{
	m_Skybox->GetShaderProgram()->ErrorChecker();
	
	SetShaderParams(m_Skybox->GetShaderProgram());
	SetLightParams(m_Skybox->GetShaderProgram());

}

void RenderEngine::Render()
{
	ClearScreen();
	
	//Create the cubemap matrices
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_ShadowWidth / (float)m_ShadowHeight, 0.1f, 1000.f);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Sun->GetPosition(), m_Sun->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Sun->GetPosition(), m_Sun->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Sun->GetPosition(), m_Sun->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Sun->GetPosition(), m_Sun->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Sun->GetPosition(), m_Sun->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Sun->GetPosition(), m_Sun->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));



	glViewport(0, 0, m_ShadowWidth, m_ShadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	// Compute the MVP matrix from the light's point of view
	SetShaderParams(m_DepthShader);
	SetLightParams(m_DepthShader);
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_DepthShader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}
	RenderSceneObjects(m_DepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, DepthMap);
	SetShaderParams(m_ShadowShader);
	SetLightParams(m_ShadowShader);
	RenderSceneObjects(m_ShadowShader);
	m_Skybox->Render();
	
	
}

void RenderEngine::RenderFrameBuffers()
{
	/*m_DefaultShader->ErrorChecker();
	m_SceneFrameBuffer->BindFrameBuffer();
	ClearScreen();
	RenderSceneObjects(m_ShadowShader);
	m_Skybox->Render();
	m_SceneFrameBuffer->UnbindFrameBuffer();

	
	glClear(GL_COLOR_BUFFER_BIT);
	m_PostProcessor->DoPostProcessing(m_SceneFrameBuffer->GetColourTexture());*/
}

void RenderEngine::RenderSceneObjects(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	
	p_ShaderProgram->Use();
	p_ShaderProgram->ErrorChecker();
	for (auto gameObject : *m_GameObjects) {
		// Make sure the object has a model and transform component - so it can be rendered.
		auto modelComponent = gameObject.second->GetComponent<ModelComponent>();
		auto transformComponent = gameObject.second->GetComponent<TransformComponent>();

		if (modelComponent != nullptr && transformComponent != nullptr)
			DrawModel(modelComponent->GetModel(), transformComponent->GetModelMatrix(), p_ShaderProgram);
	}
	//p_ShaderProgram->SetMat4("model", m_Sun->GetModelMatrix());
	//m_Sun->Render();
	
}

void RenderEngine::RenderDebugging()
{
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_DebugShader->Use();
	SetShaderParams(m_DebugShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DepthMap);
	m_DebugShader->SetInt("depthMap", 0);
	debugQuad.Render();
}

void RenderEngine::SetCamera(std::shared_ptr<CameraComponent> p_Camera)
{
	m_Camera = p_Camera;
}

void RenderEngine::SetGameObjects(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> *p_GameObjects)
{
	m_GameObjects = p_GameObjects;
	auto iter = m_GameObjects->find(typeid(PlayerCharacter));
	if (iter != m_GameObjects->end())
	{
		m_PlayerObject = iter->second;
		m_Camera = iter->second->GetComponent<CameraComponent>();
	}
}

void RenderEngine::SetDefaultShader()
{
	glUseProgram(m_DefaultShader->GetID());
}

void RenderEngine::SetLightParams(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	p_ShaderProgram->Use();

	p_ShaderProgram->ErrorChecker();
	float l_AspectRatio = m_ScreenWidth / m_ScreenHeight;
	float l_FrustrumLength = 1000.0f - 0.1f;
	m_LightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.f);
	m_LightView = glm::lookAt(m_Sun->GetPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Light Pos - Target Look at - Up Angle
	m_LightSpaceMatrix = m_LightProjection * m_LightView;

	p_ShaderProgram->SetMat4("lightSpaceMatrix", m_LightSpaceMatrix);
	p_ShaderProgram->SetVec3("objectColour", 1.0f, 0.6f, 0.61f);
	p_ShaderProgram->SetVec3("lightColour", m_Sun->GetColour());
	p_ShaderProgram->SetVec3("lightPos", m_Sun->GetPosition());
	p_ShaderProgram->ErrorChecker();
}

void RenderEngine::SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	p_ShaderProgram->Use();
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera->m_FieldOfView), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 1000.0f);
	//GenerateProjectionMatrix();

	p_ShaderProgram->ErrorChecker();
	p_ShaderProgram->SetFloat("near_plane", 0.1f);
	p_ShaderProgram->SetFloat("far_plane", 1000.f);
	p_ShaderProgram->SetMat4("projection", projection);
	p_ShaderProgram->SetMat4("view", m_Camera->GetViewMatrix());
	p_ShaderProgram->SetMat4("viewSky", glm::mat4(glm::mat3(m_Camera->GetViewMatrix())));
	p_ShaderProgram->SetVec3("viewPos", m_Camera->Position());
	p_ShaderProgram->ErrorChecker();

}

void RenderEngine::SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram, CubeMapCamera &p_Camera)
{
	p_ShaderProgram->Use();

	p_ShaderProgram->ErrorChecker();
	p_ShaderProgram->SetMat4("projection", p_Camera.GetProjectionMatrix());
	p_ShaderProgram->SetMat4("view", p_Camera.GetViewMatrix());
	p_ShaderProgram->SetMat4("viewSky", glm::mat4(glm::mat3(p_Camera.GetViewMatrix())));
	p_ShaderProgram->SetVec3("viewPos", p_Camera.GetPosition());
	p_ShaderProgram->ErrorChecker();
}

void RenderEngine::RenderText(const std::string & p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour)
{
	m_FontRenderer->RenderText(p_Text, p_XPosition, p_YPosition, p_Scale, p_Colour);
}

void RenderEngine::ClearScreen()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void RenderEngine::GenerateProjectionMatrix()
{
	m_ProjectionMatrix = glm::mat4();
	float l_YScale = (float)((1.0f / glm::tan(glm::radians(m_Camera->m_FieldOfView / 2.f))));
	float l_XScale = l_YScale / m_Camera->m_AspectRatio;
	float l_FrustrumLength = m_Camera->m_FarPlane - m_Camera->m_NearPlane;

	m_ProjectionMatrix[0][0] = l_XScale;
	m_ProjectionMatrix[1][1] = l_YScale;
	m_ProjectionMatrix[2][2] = -((m_Camera->m_FarPlane + m_Camera->m_NearPlane) / l_FrustrumLength);
	m_ProjectionMatrix[2][3] = -1;
	m_ProjectionMatrix[3][2] = -((2 * m_Camera->m_NearPlane * m_Camera->m_FarPlane) / l_FrustrumLength);
	m_ProjectionMatrix[3][3] = 0;
}

