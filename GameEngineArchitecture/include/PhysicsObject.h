#pragma once

#include <string>

#include <glm/glm.hpp>

#include "GameObject.h"
#include "BodyEnums.h"

class PhysicsObject : public GameObject {
public:
	PhysicsObject(const std::string &p_ModelName, const glm::vec3& p_Pos, const glm::vec3& p_AABBSize, 
		const glm::quat &p_ModelOrientation, const glm::vec3& p_ModelScale, float p_Mass, const BodyType &p_BodyType, 
		float p_Rest = 0.0f, float p_DragCo = 0.0f, float p_FrictionCo = 0.0f);
	~PhysicsObject() = default;

	void ApplyForce(glm::vec3 p_Force);

	void SetPosition(glm::vec3 p_Pos);

	void OnUpdate(float p_DeltaTime) override;
	void OnMessage(const std::string &p_Message) override;
};