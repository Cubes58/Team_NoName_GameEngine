#include "PhysicsObject.h"

#include "PolymorphicInstanceManager.h"

#include "ModelComponent.h"
#include "BodyComponent.h"
#include "AABBComponent.h"
#include "TransformComponent.h"

PhysicsObject::PhysicsObject(const std::string &p_ModelName, const glm::vec3& p_Pos, 
	const glm::vec3& p_AABBSize, const glm::quat &p_ModelOrientation, const glm::vec3& p_ModelScale, 
	float p_Mass, const BodyType &p_BodyType, float p_Rest, float p_DragCo, float p_FrictionCo) {

	unsigned int index = 0;
	ModelComponent modelComponent(p_ModelName);
	PolymorphicInstanceManager::Instance().m_ModelComponents.PushBack(modelComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_ModelComponents.At(index));

	BodyComponent bodyComponent(p_Pos, p_Mass, p_Rest, p_DragCo, p_FrictionCo);
	PolymorphicInstanceManager::Instance().m_BodyComponents.PushBack(bodyComponent, index);
	AddComponent<BodyComponent>(PolymorphicInstanceManager::Instance().m_BodyComponents.At(index));

	AABBComponent AABBComponent(p_Pos, p_AABBSize);
	PolymorphicInstanceManager::Instance().m_AABBComponents.PushBack(AABBComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_AABBComponents.At(index));

	TransformComponent transformComponent(p_Pos, p_ModelOrientation, p_ModelScale);
	PolymorphicInstanceManager::Instance().m_TransformComponents.PushBack(transformComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_TransformComponents.At(index));

	GetComponent<BodyComponent>()->SetType(p_BodyType);
}

void PhysicsObject::ApplyForce(glm::vec3 p_Force)
{
	GetComponent<BodyComponent>()->ApplyForce(p_Force);
}

void PhysicsObject::SetPosition(glm::vec3 p_Pos)
{
	//GetComponent<BodyComponent>()->m_Position = p_Pos;
	GetComponent<AABBComponent>()->SetPosition(p_Pos);
	GetComponent<TransformComponent>()->m_Position = p_Pos;
}

void PhysicsObject::OnUpdate(float p_DeltaTime)
{
	BodyComponent *bodyComp = GetComponent<BodyComponent>();
	//bodyComp->OnUpdate(p_DeltaTime);
	glm::vec3 l_Pos = bodyComp->m_Position;
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