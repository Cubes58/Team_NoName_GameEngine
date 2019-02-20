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

#include "ShaderProgram.h"
#include "FontRenderer.h"
#include "SkyBox.h"

#include "ModelComponent.h"
#include "CameraComponent.h"

#include "PlayerCharacter.h"
#include "StaticEnvironmentObject.h"
#include "DynamicEnvironmentObject.h"
#include "EnemyTower.h"
#include "EndLevelCollectable.h"

class RenderEngine
{

private:

	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> m_GameObjects;	//!< Stores every game object.
	std::shared_ptr<ShaderProgram> m_DefaultShader;
	std::shared_ptr<ShaderProgram> m_CurrentShader;
	std::shared_ptr<CameraComponent> m_Camera;
	int s_m_ScreenWidth, s_m_ScreenHeight;

	void InitShaders();


public:

	RenderEngine(int p_ScreenWidth,  int p_ScreenHeight);
	 std::shared_ptr<FontRenderer> m_FontRenderer;
	 std::shared_ptr<Skybox> m_Skybox;

	void DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 &p_ModelMatrix);
	void Update(double p_DeltaTime);
	void Render();


	void SetCamera(std::shared_ptr<CameraComponent> p_Camera);
	void SetGameObjects(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> p_GameObjects);
	void SetDefaultShader();
	void SetCurrentShader(std::shared_ptr<ShaderProgram> p_ShaderProgram);
	void SetLightParams(std::shared_ptr<ShaderProgram> p_ShaderProgram);
	void SetShaderParams(std::shared_ptr<ShaderProgram> p_ShaderProgram);
	void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour);

	


};


