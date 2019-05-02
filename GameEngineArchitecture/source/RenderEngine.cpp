#include "RenderEngine.h"

#include "ShaderProgram.h"
#include "FontRenderer.h"
#include "SkyBox.h"
#include "FrameBufferObject.h"
#include "PostProcessor.h"
#include "Light.h"
#include "Shadows.h"

#include "ModelComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "AABBComponent.h"

#include "Scene.h"
#include "ModelManager.h"


#include "PlayerCharacter.h"
#include "StaticEnvironmentObject.h"
#include "DynamicEnvironmentObject.h"
#include "EnemyTower.h"
#include "EndLevelCollectable.h"


void RenderEngine::Init(int p_ScreenWidth, int p_ScreenHeight)
{
	m_ScreenWidth = p_ScreenWidth;
	m_ScreenHeight = p_ScreenHeight;
	m_NearPlane = 0.1f;
	m_FarPlane = 300.f;
	m_ShadowDistance = m_ShadowWidth;
	m_IsDirectionalShadows = false;
	m_Sun = new Light(glm::vec3(10.0f, 100.0f, -10.0f), glm::vec3(1.f));
	m_ShadowRenderer = std::make_shared<Shadows>(m_ShadowWidth, m_ShadowHeight, m_IsDirectionalShadows);
	m_SceneFrameBuffer = std::make_shared<FrameBufferObject>(m_ScreenWidth, m_ScreenHeight, FrameBufferType::NONE);
	m_PostProcessor = std::make_shared<PostProcessor>();
	m_PostProcessor->InitPostProcessing();
	InitShaders();

	//OpenGL constants

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Enable alpha transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void RenderEngine::InitShaders()
{
	m_DefaultShader = std::make_shared<ShaderProgram>();
	m_DefaultShader->CompileShader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag");

	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/arial.ttf", m_ScreenWidth, m_ScreenHeight, m_DefaultShader->GetID());
	m_FontRenderer->SetShader("resources/shaders/fontShader.vert", "resources/shaders/fontShader.frag");

	//Setup Shadow Shaders
	if (m_IsDirectionalShadows)
	{
		m_ShadowShader = std::make_shared<ShaderProgram>();
		m_ShadowShader->CompileShader("resources/shaders/directShadowShader.vert", "resources/shaders/directShadowShader.frag");
	}
	else
	{
		m_ShadowShader = std::make_shared<ShaderProgram>();
		m_ShadowShader->CompileShader("resources/shaders/pointShadowShader.vert", "resources/shaders/pointShadowShader.frag");
	}
	m_QuadDebugShader = std::make_shared <ShaderProgram>();
	m_QuadDebugShader->CompileShader("resources/shaders/debugShader.vert", "resources/shaders/debugShader.frag");
	m_DebugShader = std::make_shared<ShaderProgram>();
	m_DebugShader->CompileShader("resources/shaders/physicsDebug.vert", "resources/shaders/physicsDebug.frag");
	

	
	//Setup Skybox Shaders
	m_Skybox = std::make_shared<Skybox>(150);

	m_DefaultShader->ErrorChecker();
	m_DefaultShader->Use();

}

void RenderEngine::Update(double p_DeltaTime)
{
	m_Skybox->GetShaderProgram()->ErrorChecker();
	
	SetShaderParams(m_Skybox->GetShaderProgram());
	SetLightParams(m_Skybox->GetShaderProgram());

}

void RenderEngine::Render()
{
	glEnable(GL_DEPTH_TEST);
	ClearScreen();
	
	
	m_DefaultShader->ErrorChecker();
	m_ShadowRenderer->Prepare(m_Sun->GetPosition(), m_NearPlane, m_FarPlane);
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderSceneObjects(m_ShadowRenderer->Render());
	m_ShadowRenderer->End(m_ScreenWidth, m_ScreenHeight);
	
	m_DefaultShader->ErrorChecker();
	ClearScreen();
	m_ShadowShader->Use();
	m_ShadowShader->SetInt("depthMap", m_ShadowRenderer->GetShadowMap());
	RenderSceneObjects(m_ShadowShader);


	m_DefaultShader->Use();
	m_DefaultShader->ErrorChecker();
	m_DefaultShader->SetMat4("model", m_Sun->GetModelMatrix());
	SetShaderParams(m_DefaultShader);
	SetLightParams(m_DefaultShader);
	m_Sun->Render();
	

	m_Skybox->Render();
	ImGuiRender();
	//RenderDebugging();
	
}

void RenderEngine::ImGuiRender()
{
	static bool show_test_window = true;
	static bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	/////////////////////////////////////////////////////////////
	//move to scene??
	/////////////////////////////////////////////////////////////
	// imgui
	ImGui_ImplGlfwGL3_NewFrame();
	//ImGui::NewFrame();

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{

		std::shared_ptr<TransformComponent> tc = m_PlayerObject->GetComponent<TransformComponent>();


		float* fpx = &tc->m_Position.x;
		float* fpy = &tc->m_Position.y;
		float* fpz = &tc->m_Position.z;
		//TODO aabb is 111
		//loading 

	//	//static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("X axis", fpx, 0.0f, 2.0f);
	ImGui::SliderFloat("Y axis", fpy, 0.0f, 2.0f);
	ImGui::SliderFloat("Z axis", fpz, 0.0f, 2.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	if (ImGui::Button("Add Object")) MODEL_MANAGER.GetModel("longWall");
	if (ImGui::Button("Test Window")) show_test_window ^= 1;
	//if (ImGui::Button("Another Window")) show_another_window ^= 1;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}

	// Rendering
	//int display_w, display_h;
	//glfwGetFramebufferSize(m_window, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
}

void RenderEngine::RenderFrameBuffers()
{
	m_DefaultShader->ErrorChecker();
	m_SceneFrameBuffer->BindFrameBuffer();
	ClearScreen();
	RenderSceneObjects(m_ShadowShader);
	m_Skybox->Render();
	m_SceneFrameBuffer->UnbindFrameBuffer(m_ScreenWidth, m_ScreenHeight);

	
	glClear(GL_COLOR_BUFFER_BIT);
	m_PostProcessor->DoPostProcessing(m_SceneFrameBuffer->GetColourTexture());
}

void RenderEngine::RenderSceneObjects(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	p_ShaderProgram->Use();
	p_ShaderProgram->ErrorChecker();
	SetShaderParams(p_ShaderProgram);
	SetLightParams(p_ShaderProgram);
	p_ShaderProgram->ErrorChecker();
	for (auto gameObject : *m_GameObjects) {
		// Make sure the object has a model and transform component - so it can be rendered.
		auto modelComponent = gameObject.second->GetComponent<ModelComponent>();
		auto transformComponent = gameObject.second->GetComponent<TransformComponent>();
		auto physicsComponent = gameObject.second->GetComponent<AABBComponent>();

		if (modelComponent != nullptr && transformComponent != nullptr)
			DrawModel(modelComponent->GetModel(), transformComponent->GetModelMatrix(), p_ShaderProgram);
		if (physicsComponent != nullptr && transformComponent != nullptr) {
			glm::mat4 l_ModelMatrix;
			glm::mat4 l_TransMatrix = glm::translate(glm::mat4(1.0f), transformComponent->Position());
			glm::mat4 l_ScaleMatrix = glm::scale(glm::mat4(1.0f), physicsComponent->m_Size);
			glm::mat4 l_RotMatrix = glm::mat4_cast(transformComponent->Orientation());

			l_ModelMatrix = l_TransMatrix * l_RotMatrix * l_ScaleMatrix;
			RenderPhysicsDebug(l_ModelMatrix);
		}
	}
}

void RenderEngine::DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 & p_ModelMatrix, std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	p_ShaderProgram->Use();
	p_ShaderProgram->ErrorChecker();
	p_ShaderProgram->SetMat4("model", p_ModelMatrix);
	p_ShaderProgram->ErrorChecker();
	p_Model->Render(p_ShaderProgram->GetID());
}

void RenderEngine::RenderQuadDebug()
{
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	ClearScreen();
	m_QuadDebugShader->Use();
	SetShaderParams(m_QuadDebugShader);
	m_QuadDebugShader->SetInt("depthMap", m_ShadowRenderer->GetShadowMap());
	m_DebugQuad.Render();
}

void RenderEngine::RenderPhysicsDebug(const glm::mat4 & p_ModelMatrix)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_DebugShader->Use();
	SetShaderParams(m_DebugShader);
	m_DebugShader->SetMat4("model", p_ModelMatrix);
	m_PrimitiveModels.renderCube();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderEngine::SetCamera(CameraComponent *p_Camera)
{
	m_Camera = p_Camera;
	m_Camera->SetAspectRatio(m_ScreenWidth / m_ScreenHeight);
	m_Camera->SetClipPlanes(glm::vec2(m_NearPlane, m_FarPlane));
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
	glm::mat4 lightProjection = glm::mat4(1.0);
	glm::mat4 lightView = glm::mat4(1.0);
	glm::mat4 lightSpaceMatrix = glm::mat4(1.0);

	lightProjection = glm::ortho<float>(-m_ShadowDistance, m_ShadowDistance, -m_ShadowDistance, m_ShadowDistance, m_NearPlane, m_FarPlane);
	lightView = glm::lookAt(m_Sun->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	p_ShaderProgram->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	p_ShaderProgram->SetVec3("lightColour", m_Sun->GetColour());
	p_ShaderProgram->SetVec3("lightPos", m_Sun->GetPosition());
	p_ShaderProgram->ErrorChecker();
}

void RenderEngine::SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram)
{
	p_ShaderProgram->Use();
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera->m_FieldOfView), (float)m_ScreenWidth / (float)m_ScreenHeight, m_NearPlane, m_FarPlane);
	
	p_ShaderProgram->ErrorChecker();
	p_ShaderProgram->SetFloat("near_plane", m_NearPlane);
	p_ShaderProgram->SetFloat("far_plane", m_FarPlane);
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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}


