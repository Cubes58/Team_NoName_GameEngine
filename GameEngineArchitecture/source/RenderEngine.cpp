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

	m_Sun = new Light(glm::vec3(0.0f, 10.0f, 00.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	
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
	m_ShadowShader->CompileShader("resources/shaders/shadowShader.vert", "resources/shaders/shadowShader.frag");
	m_DepthShader = std::make_shared<ShaderProgram>();
	m_DepthShader->CompileShader("resources/shaders/shadowShaderDepth.vert", "resources/shaders/shadowShaderDepth.frag");

	//Setup Skybox Shaders
	m_Skybox = std::make_shared<Skybox>(150);

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
	glEnable(GL_CLIP_DISTANCE0);
	ClearScreen();
	RenderFrameBuffers();

	//Used for debugging shaders/when not using framebuffers to render
	/*RenderSceneObjects(m_ShadowShader);
	m_Skybox->Render();*/
}

void RenderEngine::RenderFrameBuffers()
{
	/*m_ShadowFrameBuffer->BindFrameBuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderSceneObjects(m_DepthShader);
	m_ShadowFrameBuffer->UnbindFrameBuffer();

	//Binds the shadow texture to the shader
	glUseProgram(m_ShadowShader->GetID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ShadowFrameBuffer->GetDepthTexture());
	m_ShadowShader->SetInt("shadowMap", 1);*/

	m_SceneFrameBuffer->BindFrameBuffer();
	ClearScreen();
	RenderSceneObjects(m_DefaultShader);
	
	m_DefaultShader->ErrorChecker();
	m_Skybox->Render();
	m_SceneFrameBuffer->UnbindFrameBuffer();

	//glDisable(GL_CLIP_DISTANCE0);
	glClear(GL_COLOR_BUFFER_BIT);
	m_PostProcessor->DoPostProcessing(m_SceneFrameBuffer->GetColourTexture());
}

void RenderEngine::RenderSceneObjects(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	glUseProgram(p_ShaderProgram->GetID());
	SetShaderParams(p_ShaderProgram);
	SetLightParams(p_ShaderProgram);
	m_DefaultShader->ErrorChecker();
	for (auto gameObject : *m_GameObjects) {
		// Make sure the object has a model and transform component - so it can be rendered.
		auto modelComponent = gameObject.second->GetComponent<ModelComponent>();
		auto transformComponent = gameObject.second->GetComponent<TransformComponent>();

		if (modelComponent != nullptr && transformComponent != nullptr)
			DrawModel(modelComponent->GetModel(), transformComponent->GetModelMatrix(), p_ShaderProgram);
	}
	p_ShaderProgram->SetMat4("model", m_Sun->GetModelMatrix());
	m_Sun->Render();
	
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
	glUseProgram(p_ShaderProgram->GetID());

	float l_AspectRatio = m_ScreenWidth / m_ScreenHeight;
	float l_FrustrumLength = 1000.0f - 0.1f;
	m_Sun->SetProjectionMatrix(glm::vec2(0.1f, 1000.f), l_AspectRatio, m_Camera->m_FieldOfView, l_FrustrumLength);

	p_ShaderProgram->SetMat4("lightSpaceMatrix", m_Sun->GetSpaceMatrix());
	p_ShaderProgram->SetVec3("objectColour", 1.0f, 0.6f, 0.61f);
	p_ShaderProgram->SetVec3("lightColour", m_Sun->GetColour());
	p_ShaderProgram->SetVec3("lightPos", m_Sun->GetPosition());
}

void RenderEngine::SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	glUseProgram(p_ShaderProgram->GetID());
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera->m_FieldOfView), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 1000.0f);

	p_ShaderProgram->ErrorChecker();
	p_ShaderProgram->SetMat4("projection", projection);
	p_ShaderProgram->SetMat4("view", m_Camera->GetViewMatrix());
	p_ShaderProgram->SetMat4("viewSky", glm::mat4(glm::mat3(m_Camera->GetViewMatrix())));
	p_ShaderProgram->SetVec3("viewPos", m_Camera->Position());
	p_ShaderProgram->ErrorChecker();

}

void RenderEngine::SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram, CubeMapCamera &p_Camera)
{
	glUseProgram(p_ShaderProgram->GetID());

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

