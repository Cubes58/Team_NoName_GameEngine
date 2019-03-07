#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>




class Light
{
private:
	glm::vec3 m_LightPosition;
	glm::vec3 m_LightColour;
	glm::mat4 m_LightProjectionMatrix;
	glm::mat4 m_LightViewMatrix;
	glm::mat4 m_LightModelMatrix;

	unsigned int m_VaoHandler, m_VboHandler;

	void InitDebugModel();
	void GenerateModelMatrix();
	void GenerateProjectionMatrix(glm::vec2 p_ClipPlanes, float p_AspectRatio, float p_FieldOfView, float p_FrustrumLength);


public:
	Light();
	Light(glm::vec3 p_LightPosition, glm::vec3 p_LightColour);

	void Render();

	void SetPosition(glm::vec3 p_LightPosition);
	void SetPosition(float p_X, float p_Y, float p_Z);
	void SetColour(glm::vec3 p_LightColour);
	void SetColour(float p_Red, float p_Green, float p_Blue);
	void SetProjectionMatrix(glm::vec2 p_ClipPlanes, float p_AspectRatio, float p_FieldOfView, float p_FrustrumLength);

	const glm::vec3& GetPosition() const;
	const glm::vec3& GetColour() const;
	const glm::mat4& GetModelMatrix() const;
	const glm::mat4& GetSpaceMatrix();

};