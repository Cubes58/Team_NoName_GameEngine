#include "DynamicEnvironmentObject.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "MovementComponent.h"

DynamicEnvironmentObject::DynamicEnvironmentObject(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
	const glm::vec3 &p_Acceleration, const glm::vec3 &p_Velocity) {
	AddComponent(std::make_shared<ModelComponent>(p_ModelName));
	AddComponent(std::make_shared<TransformComponent>(p_Position, p_Orientation, p_Scale));
	AddComponent(std::make_shared<MovementComponent>(p_Acceleration, p_Velocity));
}

void DynamicEnvironmentObject::OnUpdate(float p_DeltaTime) {
	GetComponent<MovementComponent>()->OnUpdate(p_DeltaTime);
	GetComponent<TransformComponent>()->m_Position += GetComponent<MovementComponent>()->Velocity() * p_DeltaTime;
}

void DynamicEnvironmentObject::OnMessage(const std::string &p_Message) {

}