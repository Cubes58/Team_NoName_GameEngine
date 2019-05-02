#pragma once

#include <map>
#include <vector>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <memory>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

#include "FrameBufferType.h"
#include "VBOQuad.h"

class GameObject;
class ShaderProgram;
class CameraComponent;
class FontRenderer;
class Skybox;
class Model;
class CubeMapCamera;
class FrameBufferObject;
class PostProcessor;
class Light;
class Shadows;



#define RenderEngineInstance RenderEngine::Instance()

class RenderEngine {
private:
	RenderEngine() = default;
	~RenderEngine() = default;

	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> *m_GameObjects;	//!< Stores every game object.
	std::shared_ptr<ShaderProgram> m_DefaultShader;
	std::shared_ptr<ShaderProgram> m_ShadowShader;
	std::shared_ptr<ShaderProgram> m_DebugShader;

	std::shared_ptr<FontRenderer> m_FontRenderer;
	std::shared_ptr<Skybox> m_Skybox;
	std::shared_ptr<CameraComponent> m_Camera;
	std::shared_ptr<FrameBufferObject> m_SceneFrameBuffer;
	std::shared_ptr<PostProcessor> m_PostProcessor;
	std::shared_ptr<Shadows> m_ShadowRenderer;

	std::shared_ptr<GameObject> m_PlayerObject;

	glm::mat4 m_ProjectionMatrix;

	Light* m_Sun;

	bool m_IsDirectionalShadows = false;

	int m_ScreenWidth, m_ScreenHeight;
	const unsigned int m_ShadowWidth = 2048;
	const unsigned int m_ShadowHeight = 2048;
	int m_ShadowDistance = 1000;

	float m_NearPlane;
	float m_FarPlane;

	VBOQuad m_DebugQuad;

	void InitShaders();

public:
	
	static RenderEngine &Instance() {
		static RenderEngine s_RenderEngine;

		return s_RenderEngine;
	}

	void Init(int p_ScreenWidth, int p_ScreenHeight);

	void DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 &p_ModelMatrix, std::shared_ptr<ShaderProgram> p_ShaderProgram);
	void Update(double p_DeltaTime);
	void Render();
	void ImGuiRender();
	void RenderSceneObjects(std::shared_ptr<ShaderProgram> p_ShaderProgram);
	void RenderDebugging();
	void RenderFrameBuffers();

	void SetCamera(std::shared_ptr<CameraComponent> p_Camera);
	void SetGameObjects(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> *p_GameObjects);
	void SetDefaultShader();
	void SetLightParams(std::shared_ptr<ShaderProgram> p_ShaderProgram);
	void SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram);
	void SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram, CubeMapCamera &p_Camera);
	void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour);
	void ClearScreen();
};


