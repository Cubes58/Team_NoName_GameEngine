#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <memory>

class CameraComponent;

class ShadowBox
{
private:
	
	float m_Offset = 10;
	float m_ShadowDistance = 100;

	glm::vec4 m_UpVector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 m_ForwardVector = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

	float m_MinX, m_MinY, m_MinZ;
	float m_MaxX, m_MaxY, m_MaxZ;
	float m_FarHeight, m_FarWidth, m_NearHeight, m_NearWidth;
	float m_AspectRatio;

	std::shared_ptr<CameraComponent> m_SceneCamera;
	glm::mat4 m_LightViewMatrix;

	std::vector<glm::vec4>CalculateFrustrumPoints(glm::mat4 p_Rotation, glm::vec3 p_ForwardVector, glm::vec3 p_CenterPointNear, glm::vec3 p_CenterPointFar);
	glm::vec4 CalculateFrustrumCorner(glm::vec3 p_StartPoint, glm::vec3 p_DirectionVector, float p_Width);
	void CalculateWidthsAndHeights();

public:
	ShadowBox(std::shared_ptr<CameraComponent> p_SceneCamera, glm::mat4 p_LightViewMatrix, float p_AspectRatio);

	void Update();

	//Getters and Setters
	glm::vec3 GetCenter();
	float GetWidth();
	float GetHeight();
	float GetLength();

	void SetAspectRation();


};