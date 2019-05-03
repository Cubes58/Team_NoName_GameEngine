#include "EndLevelCollectable.h"

#include "PolymorphicInstanceManager.h"

#include "DefaultGame.h"
#include "TransformComponent.h"
#include "ModelComponent.h"

EndLevelCollectable::EndLevelCollectable(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, std::shared_ptr<DefaultGame> p_Game) {
	m_Game = p_Game;

	unsigned int index = 0;
	ModelComponent modelComponent(p_ModelName);
	PolymorphicInstanceManager::Instance().m_ModelComponents.PushBack(modelComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_ModelComponents.At(index));

	TransformComponent transformComponent(p_Position, p_Orientation, p_Scale);
	PolymorphicInstanceManager::Instance().m_TransformComponents.PushBack(transformComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_TransformComponents.At(index));
}

void EndLevelCollectable::OnUpdate(float p_DeltaTime) {

}

void EndLevelCollectable::OnMessage(const std::string &p_Message) {
	if (p_Message == "NextScene") {
		if (auto validGameObject = m_Game.lock()) 
			validGameObject->SetScene(validGameObject->GetNextScene());
	}
}