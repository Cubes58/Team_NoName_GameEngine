#include "DynamicEnvironmentObject.h"

#include "PolymorphicInstanceManager.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "MovementComponent.h"

DynamicEnvironmentObject::DynamicEnvironmentObject(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
	const glm::vec3 &p_Acceleration, const glm::vec3 &p_Velocity) {

	/*
	unsigned int index = 0;
	PolymorphicInstanceManager::Instance().m_ModelComponents.PushBack(ModelComponent(p_ModelName), index);
	AddComponent(PolymorphicInstanceManager::Instance().m_ModelComponents.At(index));

	PolymorphicInstanceManager::Instance().m_TransformComponents.PushBack(TransformComponent(p_Position, p_Orientation, p_Scale), index);
	AddComponent(PolymorphicInstanceManager::Instance().m_TransformComponents.At(index));

	PolymorphicInstanceManager::Instance().m_MovementComponents.PushBack(MovementComponent(p_Acceleration, p_Velocity), index);
	AddComponent(PolymorphicInstanceManager::Instance().m_MovementComponents.At(index));
	*/

	AddComponent(new ModelComponent(p_ModelName));
	AddComponent(new TransformComponent(p_Position, p_Orientation, p_Scale));
	AddComponent(new MovementComponent(p_Acceleration, p_Velocity));
}

void DynamicEnvironmentObject::OnUpdate(float p_DeltaTime) {
	GetComponent<MovementComponent>()->OnUpdate(p_DeltaTime);
	GetComponent<TransformComponent>()->m_Position += GetComponent<MovementComponent>()->Velocity() * p_DeltaTime;
}

void DynamicEnvironmentObject::OnMessage(const std::string &p_Message) {

}