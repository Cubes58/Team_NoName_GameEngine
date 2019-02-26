#include "RenderEngine.h"

#include "ShaderProgram.h"
#include "FontRenderer.h"
#include "SkyBox.h"
#include "FrameBufferObject.h"
#include "PostProcessor.h"

#include "ModelComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

#include "PlayerCharacter.h"
#include "StaticEnvironmentObject.h"
#include "DynamicEnvironmentObject.h"
#include "EnemyTower.h"
#include "EndLevelCollectable.h"

void RenderEngine::InitShaders()
{
	m_DefaultShader =  std::make_shared<ShaderProgram>();
	m_DefaultShader->CompileShader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag");
	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/arial.ttf", m_ScreenWidth, m_ScreenHeight, m_DefaultShader->GetID());
	m_FontRenderer->SetShader("resources/shaders/fontShader.vert", "resources/shaders/fontShader.frag");

	m_Skybox = std::make_shared<Skybox>(150);
	SetLightParams(m_Skybox->GetShaderProgram());
	SetLightParams(m_DefaultShader);
	SetDefaultShader();
}

void RenderEngine::Init(int p_ScreenWidth, int p_ScreenHeight)
{
	m_ScreenWidth = p_ScreenWidth;
	m_ScreenHeight = p_ScreenHeight;

	m_SceneFrameBuffer = std::make_shared<FrameBufferObject>(p_ScreenWidth, p_ScreenHeight, 1);
	m_PostProcessor = std::make_shared<PostProcessor>();
	m_PostProcessor->InitPostProcessing();
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
	ClearScreen();
	SetDefaultShader();
	RenderSceneObjects();
	m_Skybox->Render();
	SetDefaultShader();

	RenderFrameBuffers();
}

void RenderEngine::RenderSceneObjects()
{
	SetDefaultShader();
	for (auto gameObject : *m_GameObjects) {
		// Make sure the object has a model and transform component - so it can be rendered.
		auto modelComponent = gameObject.second->GetComponent<ModelComponent>();
		auto transformComponent = gameObject.second->GetComponent<TransformComponent>();

		if (modelComponent != nullptr && transformComponent != nullptr)
			DrawModel(modelComponent->GetModel(), transformComponent->GetModelMatrix());
	}
	m_Skybox->Render();
}

void RenderEngine::RenderFrameBuffers(CubeMapCamera &p_Camera)
{

}

void RenderEngine::RenderFrameBuffers()
{
	m_SceneFrameBuffer->BindFrameBuffer();
	ClearScreen();
	RenderSceneObjects();
	m_SceneFrameBuffer->UnbindFrameBuffer();

	//glDisable(GL_CLIP_DISTANCE0);
	glClear(GL_COLOR_BUFFER_BIT);
	m_PostProcessor->DoPostProcessing(m_SceneFrameBuffer->GetColourTexture());
	

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
		m_Camera = iter->second->GetComponent<CameraComponent>();
	}
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
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera->m_FieldOfView), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 300.0f);

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

