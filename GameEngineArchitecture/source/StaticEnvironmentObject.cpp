#include "StaticEnvironmentObject.h"

#include "PolymorphicInstanceManager.h"

#include "TransformComponent.h"
#include "ModelComponent.h"

StaticEnvironmentObject::StaticEnvironmentObject(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale) {
	unsigned int index = 0;
	ModelComponent modelComponent(p_ModelName);
	PolymorphicInstanceManager::Instance().m_ModelComponents.PushBack(modelComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_ModelComponents.At(index));

	TransformComponent transformComponent(p_Position, p_Orientation, p_Scale);
	PolymorphicInstanceManager::Instance().m_TransformComponents.PushBack(transformComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_TransformComponents.At(index));
}

void StaticEnvironmentObject::OnUpdate(float p_DeltaTime) {

}

void StaticEnvironmentObject::OnMessage(const std::string &p_Message) {
	
}