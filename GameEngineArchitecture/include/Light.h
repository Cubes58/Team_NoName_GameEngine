#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




class Light
{
private:
	glm::vec3 m_LightPosition;
	glm::vec3 m_LightColour;
	glm::vec3 m_FrontVector;
	glm::vec3 m_RightVector;
	glm::vec3 m_UpVector;
	glm::mat4 m_LightProjectionMatrix;
	glm::mat4 m_LightViewMatrix;
	glm::mat4 m_LightModelMatrix;

	unsigned int m_VaoHandler, m_VboHandler;

	float m_Pitch;
	float m_Yaw;

	void InitDebugModel();
	void GenerateModelMatrix();
	void GenerateProjectionMatrix();
	void GenerateViewMatrix();


public:
	Light();
	Light(glm::vec3 p_LightPosition, glm::vec3 p_LightColour);
	Light(glm::vec3 p_LightPosition, glm::vec3 p_LightColour, float p_Pitch, float p_Yaw);

	void Render();

	void SetPosition(glm::vec3 p_LightPosition);
	void SetPosition(float p_X, float p_Y, float p_Z);
	void SetColour(glm::vec3 p_LightColour);
	void SetColour(float p_Red, float p_Green, float p_Blue);
	void SetProjectionMatrix();
	void SetViewMatrix();
	void SetRotation(float p_Pitch, float p_Yaw);

	const glm::vec3& GetPosition() const;
	const glm::vec3& GetColour() const;
	const glm::mat4& GetModelMatrix() const;
	const glm::mat4& GetViewMatrix();

};