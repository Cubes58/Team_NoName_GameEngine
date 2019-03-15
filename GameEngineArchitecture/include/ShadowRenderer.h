#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <memory>

class ShadowBox;
class ShaderProgram;
class FrameBufferObject;
class CameraComponent;
class RenderEngine;

class ShadowRenderer
{

private:

	//Shadow Matrices
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_LightViewMatrix;
	glm::mat4 m_ProjViewMatrix;
	glm::mat4 m_OffsetMatrix;

	ShadowBox* m_ShadowBox;
	FrameBufferObject* m_FrameBuffer;
	std::shared_ptr<ShaderProgram> m_ShadowShader;

	RenderEngine *m_RenderEngine = nullptr; //!< Pointer to the engine for rendering 

	void Prepare(glm::vec3 p_SunPosition);
	void UpdateLightViewMatrix(glm::vec3 p_Direction, glm::vec3 p_Center);
	void UpdateOrthoProjectionMatrix(float p_Width, float p_Height, float p_Length);
	void CreateOffsetMatrix();

public:
	ShadowRenderer(int p_ShadowSize, RenderEngine *p_RenderEngine);

	int GetShadowMap();
	glm::mat4 GetLightViewMatrix();
	glm::mat4 GetShadowSpaceMatrix();

	void SetCamera(std::shared_ptr<CameraComponent> p_Camera);

	void Render(glm::vec3 p_SunPosition);
	void SetShaderParams();
};