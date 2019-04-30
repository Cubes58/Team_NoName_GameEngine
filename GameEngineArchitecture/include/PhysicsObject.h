#pragma once

#pragma once

#include <string>
#include <glm/glm.hpp>
#include "GameObject.h"

class PhysicsObject : public GameObject {
public:
	PhysicsObject(const std::string &p_ModelName, const glm::vec3& p_Pos, const glm::vec3& p_AABBSize, const glm::quat &p_ModelOrientation, const glm::vec3& p_ModelScale, float p_Mass, float p_Rest, float p_DragCo, float p_FrictionCo);
	~PhysicsObject() = default;

	void ApplyForce(glm::vec3 p_Force);

	void SetPosition(glm::vec3 p_Pos);

	void OnUpdate(float p_DeltaTime) override;

	void OnMessage(const std::string &p_Message) override;
};