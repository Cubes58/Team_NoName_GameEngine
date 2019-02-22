#include "..\include\CubemapCamera.h"

void CubeMapCamera::CreateProjectionMatrix()
{
	float l_YScale = (float)((1.f / glm::tan(glm::radians(m_FOV / 2.f))));
	float l_XScale = l_YScale / m_AspectRatio;
	float l_FrustumLength = m_FarPlane - m_NearPlane;

	m_ProjectionMatrix[0][0] = l_XScale;
	m_ProjectionMatrix[1][1] = l_YScale;
	m_ProjectionMatrix[2][2] = -((m_FarPlane + m_NearPlane) / l_FrustumLength);
	m_ProjectionMatrix[2][3] = -1;
	m_ProjectionMatrix[3][2] = -((2 * m_NearPlane * m_FarPlane) / l_FrustumLength);
	m_ProjectionMatrix[3][3] = 0;
}

void CubeMapCamera::UpdateViewMatrix()
{
	m_ViewMatrix = glm::mat4(1.0);
	m_ViewMatrix = glm::rotate(m_ViewMatrix, 180.f, glm::vec3(0, 0, 1));
	m_ViewMatrix = glm::rotate(m_ViewMatrix, m_Pitch, glm::vec3(1, 0, 0));
	m_ViewMatrix = glm::rotate(m_ViewMatrix, m_Yaw, glm::vec3(0, 1, 0));
	glm::vec3 l_NegativeCameraPos = glm::vec3(-m_CenterPoint.x, -m_CenterPoint.y, -m_CenterPoint.z);
	m_ViewMatrix = glm::translate(m_ViewMatrix, l_NegativeCameraPos);

	m_ProjectionViewMatrix = glm::matrixCompMult(m_ProjectionMatrix, m_ViewMatrix);
}

CubeMapCamera::CubeMapCamera()
{
	m_CenterPoint = glm::vec3(0, 0, 0);
	CreateProjectionMatrix();
}

CubeMapCamera::CubeMapCamera(float p_NearPlane, float p_FarPlane)
{
	m_NearPlane = p_NearPlane;
	m_FarPlane = p_FarPlane;

	m_CenterPoint = glm::vec3(0, 0, 0);
	CreateProjectionMatrix();
}

void CubeMapCamera::SwitchToFace(int p_FaceIndex)
{
	switch (p_FaceIndex) {
	case 0:
		m_Pitch = 0;
		m_Yaw = 90;
		break;
	case 1:
		m_Pitch = 0;
		m_Yaw = -90;
		break;
	case 2:
		m_Pitch = -90;
		m_Yaw = 180;
		break;
	case 3:
		m_Pitch = 90;
		m_Yaw = 180;
		break;
	case 4:
		m_Pitch = 0;
		m_Yaw = 180;
		break;
	case 5:
		m_Pitch = 0;
		m_Yaw = 0;
		break;
	}
	UpdateViewMatrix();
}

void CubeMapCamera::SetPosition(glm::vec3 p_Position)
{
	m_CenterPoint = p_Position;
}

void CubeMapCamera::SetFOV(float p_FOV)
{
	m_FOV = p_FOV;
}

void CubeMapCamera::SetAspectRatio(float p_AspectRatio)
{
	m_AspectRatio = p_AspectRatio;
}

void CubeMapCamera::SetClipPlanes(float p_NearPlane, float p_FarPlane)
{
	m_NearPlane = p_NearPlane;
	m_FarPlane = p_FarPlane;
}

glm::vec3 CubeMapCamera::GetPosition()
{
	return m_CenterPoint;
}

glm::mat4 CubeMapCamera::GetViewMatrix()
{
	return m_ViewMatrix;
}

glm::mat4 CubeMapCamera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

glm::mat4 CubeMapCamera::GetProjectionViewMatrix()
{
	return m_ProjectionViewMatrix;
}
