#include "ShadowBox.h"
#include "CameraComponent.h"


std::vector<glm::vec4> ShadowBox::CalculateFrustrumPoints(glm::mat4 p_Rotation, glm::vec3 p_ForwardVector, glm::vec3 p_CenterPointNear, glm::vec3 p_CenterPointFar)
{
	glm::vec3 l_UpVector = glm::vec3(m_UpVector * p_Rotation);
	glm::vec3 l_RightVector = glm::cross(p_ForwardVector, l_UpVector);
	glm::vec3 l_DownVector = glm::vec3(-l_UpVector.x, -l_UpVector.y, -l_UpVector.z);
	glm::vec3 l_LeftVector = glm::vec3(-l_RightVector.x, -l_RightVector.y, -l_RightVector.z);

	glm::vec3 l_FarTop = glm::vec3(p_CenterPointFar + glm::vec3(l_UpVector.x * m_FarHeight,
		l_UpVector.y * m_FarHeight, l_UpVector.z * m_FarHeight));

	glm::vec3 l_FarBottom = glm::vec3(p_CenterPointFar + glm::vec3(l_DownVector.x * m_FarHeight,
		l_DownVector.y * m_FarHeight, l_DownVector.z * m_FarHeight));

	glm::vec3 l_NearTop = glm::vec3(p_CenterPointNear + glm::vec3(l_UpVector.x * m_NearHeight,
		l_UpVector.y * m_NearHeight, l_UpVector.z * m_NearHeight));

	glm::vec3 l_NearBottom = glm::vec3(p_CenterPointNear + glm::vec3(l_DownVector.x * m_NearHeight,
		l_DownVector.y * m_NearHeight, l_DownVector.z * m_NearHeight));

	std::vector<glm::vec4> l_Points;
	l_Points.resize(8);
	l_Points[0] = CalculateFrustrumCorner(l_FarTop, l_RightVector, m_FarWidth);
	l_Points[1] = CalculateFrustrumCorner(l_FarTop, l_LeftVector, m_FarWidth);
	l_Points[2] = CalculateFrustrumCorner(l_FarBottom, l_RightVector, m_FarWidth);
	l_Points[3] = CalculateFrustrumCorner(l_FarBottom, l_LeftVector, m_FarWidth);

	l_Points[4] = CalculateFrustrumCorner(l_NearTop, l_RightVector, m_NearWidth);
	l_Points[5] = CalculateFrustrumCorner(l_NearTop, l_LeftVector, m_NearWidth);
	l_Points[6] = CalculateFrustrumCorner(l_NearBottom, l_RightVector, m_NearWidth);
	l_Points[7] = CalculateFrustrumCorner(l_NearBottom, l_LeftVector, m_NearWidth);

	return l_Points;
}

glm::vec4 ShadowBox::CalculateFrustrumCorner(glm::vec3 p_StartPoint, glm::vec3 p_DirectionVector, float p_Width)
{
	glm::vec3 l_Point = p_StartPoint + glm::vec3(p_DirectionVector * p_Width);
	return glm::vec4(glm::vec4(l_Point, 1.0f) * m_LightViewMatrix);
}

void ShadowBox::CalculateWidthsAndHeights()
{
	m_FarWidth = (float)(m_ShadowDistance * glm::tan(glm::radians(m_SceneCamera->m_FieldOfView)));
	m_NearWidth = (float)(0.1f * glm::tan(glm::radians(m_SceneCamera->m_FieldOfView)));
	m_FarHeight = m_FarWidth / m_AspectRatio;
	m_NearHeight = m_NearWidth / m_AspectRatio;
}

ShadowBox::ShadowBox(std::shared_ptr<CameraComponent> p_SceneCamera, glm::mat4 p_LightViewMatrix)
{
	this->m_LightViewMatrix = p_LightViewMatrix;
	this->m_SceneCamera = p_SceneCamera;
	this->m_AspectRatio = p_SceneCamera->m_AspectRatio;

	CalculateWidthsAndHeights();
}

void ShadowBox::Update()
{
	glm::mat4 l_Rotation = glm::toMat4(m_SceneCamera->Orientation());
	glm::vec3 l_ForwardVector = glm::vec3(m_ForwardVector * l_Rotation);

	glm::vec3 l_FarPoint = l_ForwardVector * m_ShadowDistance; // Far Plane replaced by distance to render shadows
	glm::vec3 l_NearPoint = l_ForwardVector * 0.1f; //Near Plane
	
	glm::vec3 l_CenterPointNear = l_NearPoint + m_SceneCamera->Position();
	glm::vec3 l_CenterPointFar  = l_FarPoint  + m_SceneCamera->Position();

	std::vector<glm::vec4> l_BoxPoints = CalculateFrustrumPoints(l_Rotation, l_ForwardVector, l_CenterPointNear, l_CenterPointFar);

	bool l_First = true;
	for (glm::vec4 point : l_BoxPoints)
	{
		if (l_First)
		{
			m_MinX = point.x;
			m_MaxX = point.x;
			m_MinY = point.y;
			m_MaxY = point.y;
			m_MinZ = point.z;
			m_MaxZ = point.z;
			l_First = false;
			continue;
		}
		if (point.x > m_MaxX){
			m_MaxX = point.x;
		}
		else if (point.x < m_MinX) {
			m_MinX = point.x;
		}
		if (point.y > m_MaxY) {
			m_MaxY = point.y;
		}
		else if (point.y < m_MinY) {
			m_MinY = point.y;
		}
		if (point.z > m_MaxZ) {
			m_MaxZ = point.z;
		}
		else if (point.z < m_MinZ) {
			m_MinZ = point.z;
		}
	}
	m_MaxZ += m_Offset;
}

glm::vec3 ShadowBox::GetCenter()
{
	float l_X = (m_MinX + m_MaxX) / 2.f;
	float l_Y = (m_MinY + m_MaxY) / 2.f;
	float l_Z = (m_MinZ + m_MaxZ) / 2.f;

	glm::vec4 l_CenterPoint = glm::vec4(l_X, l_Y, l_Z, 1.0f);
	glm::inverse(m_LightViewMatrix);

	return glm::vec3(l_CenterPoint * m_LightViewMatrix);
}

float ShadowBox::GetWidth()
{
	return m_MaxX - m_MinX;
}

float ShadowBox::GetHeight()
{
	return m_MaxY - m_MinY;
}

float ShadowBox::GetLength()
{
	return m_MaxZ - m_MinZ;
}
