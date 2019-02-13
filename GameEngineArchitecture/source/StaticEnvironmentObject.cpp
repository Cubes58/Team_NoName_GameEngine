#include "StaticEnvironmentObject.h"

#include "TransformComponent.h"
#include "ModelComponent.h"

StaticEnvironmentObject::StaticEnvironmentObject(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale) {
	AddComponent(std::make_shared<ModelComponent>(p_ModelName));
	AddComponent(std::make_shared<TransformComponent>(p_Position, p_Orientation, p_Scale));
}

void StaticEnvironmentObject::OnUpdate(float p_DeltaTime) {

}

void StaticEnvironmentObject::OnMessage(const std::string &p_Message) {
	
}