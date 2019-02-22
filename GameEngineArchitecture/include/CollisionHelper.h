#pragma once

#include <cmath>
#include <memory>
#include <glm/glm.hpp>
#include "CollisionEnums.h"
#include "GameObject.h"

class CollisionHelper {
private:
	float m_PlaneThickness = 1.0f;
public:
	CollisionEnums HalfSpaceTest(glm::vec3 p_PointTest, glm::vec3 p_PointPlane, glm::vec3 p_Normal);
	CollisionEnums HalfSpaceTest(glm::vec3 p_PointTest, glm::vec3 p_Normal, float p_Offset);
	bool PointSphere(float p_Radius, glm::vec3 p_CirclePos, glm::vec3 p_Point);
	bool SphereSphere(float p_ARadius, glm::vec3 p_ACirclePos, float p_BRadius, glm::vec3 p_BCirclePos);
	bool SpherePlane(float p_Radius, glm::vec3 p_CirclePos, glm::vec3 p_Point, glm::vec3 p_Normal);
	bool PointAABB(glm::vec3 p_CornerA, glm::vec3 p_CornerB, glm::vec3 p_Point);
	bool AABBAABB(glm::vec3 p_ACornerA, glm::vec3 p_ACornerB, glm::vec3 p_BCornerA, glm::vec3 p_BCornerB);
};