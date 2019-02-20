#pragma once
#include <vector>
#include <map>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include "ShaderProgram.h"
#include "FontRenderer.h"

class Game;
class Model;
class CameraComponent;

class RenderEngine
{

private:

	FontRenderer *m_FontRenderer;
	ShaderProgram m_DefaultShader;
	std::shared_ptr<CameraComponent> m_Camera;
	static int s_m_ScreenWidth, s_m_ScreenHeight;

	

	void InitShaders();
	void SetLightParams();
	void SetShaderParams();
	void SetDefaultShader();


public:

	RenderEngine(int p_ScreenWidth,  int p_ScreenHeight);

	void DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 &p_ModelMatrix);
	void Update(double p_DeltaTime);


	void SetCamera(std::shared_ptr<CameraComponent> p_Camera);
	


};


