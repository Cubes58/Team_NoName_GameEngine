#include "EndLevelCollectable.h"

#include "DefaultGame.h"
#include "TransformComponent.h"
#include "ModelComponent.h"

EndLevelCollectable::EndLevelCollectable(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, std::shared_ptr<DefaultGame> p_Game) {
	m_Game = p_Game;

	AddComponent(new ModelComponent(p_ModelName));
	AddComponent(new TransformComponent(p_Position, p_Orientation, p_Scale));
}

void EndLevelCollectable::OnUpdate(float p_DeltaTime) {

}

void EndLevelCollectable::OnMessage(const std::string &p_Message) {
	if (p_Message == "NextScene") {
		if (auto validGameObject = m_Game.lock()) 
			validGameObject->SetScene(validGameObject->GetNextScene());
	}
}