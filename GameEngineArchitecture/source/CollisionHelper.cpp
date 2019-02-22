#include "..\include\CollisionHelper.h"

CollisionEnums CollisionHelper::HalfSpaceTest(glm::vec3 p_PointTest, glm::vec3 p_PointPlane, glm::vec3 p_Normal)
{
	glm::vec3 l_Vec = p_PointTest - p_PointPlane;
	float l_Dist = glm::dot(l_Vec, p_Normal);
	if (l_Dist > m_PlaneThickness) {
		return CollisionEnums::INFRONT_OF_PLANE;
	}
	else if (l_Dist < -m_PlaneThickness) {
		return CollisionEnums::BEHIND_OF_PLANE;
	}
	return CollisionEnums::ON_PLANE;
}

CollisionEnums CollisionHelper::HalfSpaceTest(glm::vec3 p_PointTest, glm::vec3 p_Normal, float p_Offset)
{
	float l_Dist = glm::dot(p_PointTest, p_Normal) - p_Offset;
	if (l_Dist > m_PlaneThickness) {
		return CollisionEnums::INFRONT_OF_PLANE;
	}
	else if (l_Dist < -m_PlaneThickness) {
		return CollisionEnums::BEHIND_OF_PLANE;
	}
	return CollisionEnums::ON_PLANE;
}

bool CollisionHelper::PointSphere(float p_Radius, glm::vec3 p_CirclePos, glm::vec3 p_Point)
{
	glm::vec3 l_Vec = p_CirclePos - p_Point;
	float l_DistSqr = glm::dot(l_Vec, l_Vec);
	if (l_DistSqr < p_Radius * p_Radius) {
		return true;
	}
	return false;
}

bool CollisionHelper::SphereSphere(float p_ARadius, glm::vec3 p_ACirclePos, float p_BRadius, glm::vec3 p_BCirclePos)
{
	glm::vec3 l_Vec = p_BCirclePos - p_ACirclePos;
	float l_DistSqr = glm::dot(l_Vec, l_Vec);
	float l_RadiusTotalSqr = p_ARadius + p_BRadius;
	l_RadiusTotalSqr *= l_RadiusTotalSqr;
	if (l_DistSqr <= l_RadiusTotalSqr) {
		return true;
	}
	return false;
}

bool CollisionHelper::SpherePlane(float p_Radius, glm::vec3 p_CirclePos, glm::vec3 p_Point, glm::vec3 p_Normal)
{
	glm::vec3 l_Vec = p_CirclePos - p_Point;
	float l_Dist = glm::dot(l_Vec, p_Normal);
	if (l_Dist > p_Radius) {
		return true;
	}

	return false;
}

bool CollisionHelper::PointAABB(glm::vec3 p_CornerA, glm::vec3 p_CornerB, glm::vec3 p_Point)
{
	if (p_Point.x > p_CornerA.x and p_Point.x < p_CornerB.x and
		p_Point.y > p_CornerA.y and p_Point.y < p_CornerB.y and
		p_Point.z > p_CornerA.z and p_Point.z < p_CornerB.z) {
		return true;
	}
	return false;
}

bool CollisionHelper::AABBAABB(glm::vec3 p_ACornerA, glm::vec3 p_ACornerB, glm::vec3 p_BCornerA, glm::vec3 p_BCornerB)
{
	if (p_ACornerA.x > p_BCornerB.x and
		p_ACornerB.x > p_BCornerA.x and
		p_ACornerA.y > p_BCornerB.y and
		p_ACornerB.y > p_BCornerA.y and
		p_ACornerA.z > p_BCornerB.z and
		p_ACornerB.z > p_BCornerA.z) {
		return true;
	}
	return false;
}
