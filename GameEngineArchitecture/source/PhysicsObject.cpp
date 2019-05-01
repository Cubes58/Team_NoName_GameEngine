#include "PhysicsObject.h"

#include "ModelComponent.h"
#include "BodyComponent.h"
#include "AABBComponent.h"
#include "TransformComponent.h"

PhysicsObject::PhysicsObject(const std::string &p_ModelName, const glm::vec3& p_Pos, 
	const glm::vec3& p_AABBSize, const glm::quat &p_ModelOrientation, const glm::vec3& p_ModelScale, 
	float p_Mass, const BodyType &p_BodyType, float p_Rest, float p_DragCo, float p_FrictionCo) {

	AddComponent(std::make_shared<ModelComponent>(p_ModelName));
	AddComponent(std::make_shared<BodyComponent>(p_Pos, p_Mass, p_Rest, p_DragCo, p_FrictionCo));
	AddComponent(std::make_shared<AABBComponent>(p_Pos, p_AABBSize));
	AddComponent(std::make_shared<TransformComponent>(p_Pos, p_ModelOrientation, p_ModelScale));

	GetComponent<BodyComponent>()->SetType(p_BodyType);
}

void PhysicsObject::ApplyForce(glm::vec3 p_Force)
{
	GetComponent<BodyComponent>()->ApplyForce(p_Force);
}

void PhysicsObject::SetPosition(glm::vec3 p_Pos)
{
	GetComponent<BodyComponent>()->m_Position = p_Pos;
	GetComponent<AABBComponent>()->SetPosition(p_Pos);
	GetComponent<TransformComponent>()->m_Position = p_Pos;
}

void PhysicsObject::OnUpdate(float p_DeltaTime)
{
	GetComponent<BodyComponent>()->OnUpdate(p_DeltaTime);
	glm::vec3 l_Pos = GetComponent<BodyComponent>()->m_Position;
	SetPosition(l_Pos);
}

void PhysicsObject::OnMessage(const std::string &p_Message) {
	if (p_Message == "static") {
		GetComponent<BodyComponent>()->SetType(BodyType::STATIC);
	}
	else if (p_Message == "kinematic") {
		GetComponent<BodyComponent>()->SetType(BodyType::KINEMATIC);
	}
	else if (p_Message == "dynamic") {
		GetComponent<BodyComponent>()->SetType(BodyType::DYNAMIC);
	}
}
