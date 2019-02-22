#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//!< Basic camera setup for Cube Mapping using FBOs
class CubeMapCamera
{
private:
	//Fixed variables
	float m_NearPlane = 0.1f;
	float m_FarPlane = 200.f;
	float m_FOV = 90;
	float m_AspectRatio = 1;

	//Position and rotation
	glm::vec3 m_CenterPoint;
	float m_Pitch = 0;
	float m_Yaw = 0;

	//Transform Matrices
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionViewMatrix;

	//Updaters
	void CreateProjectionMatrix();
	void UpdateViewMatrix();

public:

	//Position is set at construction
	CubeMapCamera();
	CubeMapCamera(float p_NearPlane, float p_FarPlane);

	//Used to move the camera uniformally for a cube map
	void SwitchToFace(int p_FaceIndex);

	//Basic Setters
	void SetPosition(glm::vec3 p_Position);
	void SetFOV(float p_FOV);
	void SetAspectRatio(float p_AspectRatio);
	void SetClipPlanes(float p_NearPlane, float p_FarPlane);

	//Basic getters
	glm::vec3 GetPosition();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetProjectionViewMatrix();

};