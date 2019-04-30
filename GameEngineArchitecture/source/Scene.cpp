 #include "Scene.h"

#include <json/json.h>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "IEngineCore.h"
#include "RenderEngine.h"
#include "ModelManager.h"

#include "PlayerCharacter.h"
#include "StaticEnvironmentObject.h"
#include "DynamicEnvironmentObject.h"
#include "EnemyTower.h"
#include "EndLevelCollectable.h"

#include "ModelComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "HealthComponent.h"
#include "AttackComponent.h"

Scene::Scene(const std::string &p_SceneFile, std::shared_ptr<DefaultGame> p_Game, glm::vec3 p_BackgroundColour) : m_BackgroundColour(p_BackgroundColour) {
	LoadLevelJSON(p_SceneFile, p_Game);
}

bool Scene::LoadLevelJSON(const std::string &p_SceneFile, std::shared_ptr<DefaultGame> p_Game) {
	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	try {
		jsonData.open(p_SceneFile.c_str());
		// Check for errors, when parsing the file data.
		if (!reader.parse(jsonData, root)) {
			std::cerr << "Failed to parse data from: " << p_SceneFile << reader.getFormattedErrorMessages() << "\n";
			throw std::invalid_argument("Couldn't parse file.");
		}

		if (jsonData.is_open())
			jsonData.close();
	}
	catch (const std::invalid_argument &p_Exception) {
		std::cerr << "INVALID_ARGUMENT::INPUT_HANDLER: " << p_Exception.what() << "\n";
		if (jsonData.is_open())
			jsonData.close();
		return false;
	}
	catch (...) {
		// Catch all rune-time errors, and attempt to recover.
		std::cerr << "ERROR::INPUT_HANDLER::Something unexpected happened, while reading the key configuration file: " << p_SceneFile
			<< "\nAttempting to load the key configuration file again!" << std::endl;
		if (jsonData.is_open())
			jsonData.close();
		return false;
	}
	const Json::Value gameObjects = root["GameObjects"];

	// Resize the vector of game objects, to how many are in the file.
	m_GameObjects.reserve(gameObjects.size());
	for (unsigned int i = 0; i < gameObjects.size(); i++) {
		// Get the object's model name.
		const Json::Value modelNode = gameObjects[i]["modelName"];
		std::string modelName = "default";
		if (modelNode.type() != Json::nullValue) {
			modelName = modelNode.asString();
		}

		// Get the position node.
		glm::vec3 position(0.0f, 0.0f, 0.0f);
		const Json::Value positionNode = gameObjects[i]["position"];
		if (positionNode.type() != Json::nullValue) {
			position.x = positionNode[0].asFloat();	
			position.y = positionNode[1].asFloat();	
			position.z = positionNode[2].asFloat();	
		}		

		// Get the orientation node.
		glm::quat orientation(1.0f, 0.0f, 0.0f, 0.0f);
		const Json::Value orientationNode = gameObjects[i]["orientation"];
		if (orientationNode.type() != Json::nullValue) {
			orientation.w = orientationNode[0].asFloat();
			orientation.x = orientationNode[1].asFloat();
			orientation.y = orientationNode[2].asFloat();
			orientation.z = orientationNode[3].asFloat();
		}

		// Get the scale node.
		glm::vec3 scale(1.0f, 1.0f, 1.0f);
		const Json::Value scaleNode = gameObjects[i]["scale"];
		if (scaleNode.type() != Json::nullValue) {
			scale.x = scaleNode[0].asFloat();
			scale.y = scaleNode[1].asFloat();
			scale.z = scaleNode[2].asFloat();
		}

		// Get the acceleration node.
		glm::vec3 acceleration(0.0f, 0.0f, 0.0f);
		const Json::Value accelerationNode = gameObjects[i]["acceleration"];
		if (accelerationNode.type() != Json::nullValue) {
			acceleration.x = accelerationNode[0].asFloat();
			acceleration.y = accelerationNode[1].asFloat();
			acceleration.z = accelerationNode[2].asFloat();
		}

		// Get the velocity node.
		glm::vec3 velocity(0.0f, 0.0f, 0.0f);
		const Json::Value velocityNode = gameObjects[i]["velocity"];
		if (velocityNode.type() != Json::nullValue) {
			velocity.x = velocityNode[0].asFloat();
			velocity.y = velocityNode[1].asFloat();
			velocity.z = velocityNode[2].asFloat();
		}

		// Get the damage node.
		float attackDamage(2.5f);
		const Json::Value attackDamageNode = gameObjects[i]["attackDamage"];
		if (attackDamageNode.type() != Json::nullValue) {
			attackDamage = attackDamageNode.asFloat();
		}

		// Get the speed node.
		float attackSpeed(15.0f);
		const Json::Value attackSpeedNode = gameObjects[i]["attackSpeed"];
		if (attackSpeedNode.type() != Json::nullValue) {
			attackSpeed = attackSpeedNode.asFloat();
		}

		// Get the attack distance node.
		float attackDistance(30.0f);
		const Json::Value attackDistanceNode = gameObjects[i]["attackDistance"];
		if (attackDistanceNode.type() != Json::nullValue) {
			attackDistance = attackDistanceNode.asFloat();
		}

		// Get the health node.
		float health(100.0f);
		const Json::Value healthNode = gameObjects[i]["health"];
		if (healthNode.type() != Json::nullValue) {
			health = healthNode.asFloat();
		}

		// Get the max health node.
		float maxHealth(100.0f);
		const Json::Value maxHealthNode = gameObjects[i]["maxHealth"];
		if (maxHealthNode.type() != Json::nullValue) {
			maxHealth = maxHealthNode.asFloat();
		}

		// Get the uniform speed node.
		float uniformMovementSpeed(0.35f);
		const Json::Value uniformMovementSpeedNode = gameObjects[i]["uniformMovementSpeed"];
		if (uniformMovementSpeedNode.type() != Json::nullValue) {
			uniformMovementSpeed = uniformMovementSpeedNode.asFloat();
		}

		// Get the type node.
		const Json::Value typeNode = gameObjects[i]["type"];
		std::string type = "Unknown";
		if (typeNode.type() != Json::nullValue) {
			type = typeNode.asString();
		}

		if (type == "PlayerCharacter") {
			m_GameObjects.emplace(typeid(PlayerCharacter), std::make_shared<PlayerCharacter>(modelName, position, orientation, scale, health, uniformMovementSpeed));
			auto iter = m_GameObjects.find(typeid(PlayerCharacter));
			if (iter != m_GameObjects.end())
				m_ObjectsRequiringInput.insert(*iter);
		}
		else if (type == "StaticEnvironmentObject") {
			m_GameObjects.emplace(typeid(StaticEnvironmentObject), std::make_shared<StaticEnvironmentObject>(modelName, position, orientation, scale));
		}
		else if (type == "DynamicEnvironmentObject") {
			m_GameObjects.emplace(typeid(DynamicEnvironmentObject), std::make_shared<DynamicEnvironmentObject>(modelName, position, orientation, scale, acceleration, velocity));
		}
		else if (type == "EnemyTower") {
			m_GameObjects.emplace(typeid(EnemyTower), std::make_shared<EnemyTower>(modelName, position, orientation, scale, attackDamage, attackDistance, attackSpeed, health));
		}
		else if (type == "EndLevelCollectable") {
			m_GameObjects.emplace(typeid(EndLevelCollectable), std::make_shared<EndLevelCollectable>(modelName, position, orientation, scale, p_Game));
		}
		else {
			std::cout << "ERROR::CANNOT LOAD OBJECT!" << 
				"\nOBJECT MODEL NAME: " << modelName << 
				"\nOBJECT TYPE: " << type << std::endl;
			continue;
		}
		
	}
	RenderEngineInstance.SetGameObjects(&m_GameObjects);
	return true;
}

void Scene::UnloadLevel() {
	// Memory should clean itself (smart pointers). Just clear the references.
	m_GameObjects.clear();
	m_ObjectsRequiringInput.clear();

}

void Scene::Render(std::shared_ptr<IEngineCore> p_EngineInterface) {

	RenderEngineInstance.Render();

	auto iter = m_GameObjects.find(typeid(PlayerCharacter));
	if (iter != m_GameObjects.end()) {

		// Player GUI information.
		glm::vec3 playerPosition = iter->second->GetComponent<TransformComponent>()->Position();
		RenderEngineInstance.RenderText(std::to_string((int)playerPosition.x) + " " + std::to_string((int)playerPosition.y) + " " + std::to_string((int)playerPosition.z), 0.05f, 0.025f, 0.45f, glm::vec3(0.5f, 1.0f, 0.0f));
		RenderEngineInstance.RenderText("Health: " + std::to_string(static_cast<int>(iter->second->GetComponent<HealthComponent>()->GetHealth())), 0.85f, 0.025f, 0.45f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	RenderEngineInstance.RenderText("Get the British flag!", 0.755f, 0.955f, 0.45f, glm::vec3(0.2f, 0.5f, 0.2f));

	DisplayUnsuccessfullyLoadedModels(p_EngineInterface);
}

void Scene::Update(float p_DeltaTime) {
	// Checks whether the player is within the range of any enemy towers.
	auto defenceTowers = m_GameObjects.equal_range(typeid(EnemyTower));
	auto playerCharacter = m_GameObjects.find(typeid(PlayerCharacter));
	for (auto iter = defenceTowers.first; iter != defenceTowers.second; ++iter) {
		if (m_Collision(playerCharacter->second->GetComponent<TransformComponent>()->m_Position, iter->second->GetComponent<TransformComponent>()->m_Position, glm::vec3(7.5f, 7.5f, 7.5f), glm::vec3(40.0f, 40.0f, 40.0f))) {
			if (iter->second->GetComponent<AttackComponent>()->CanAttack()) {
				iter->second->OnMessage("Attack");
				playerCharacter->second->OnMessage("HitByEnemyTower");
			}
		}
	}

	// Check whether the player is within the range of the EndLevelCollectable object.
	auto endLevelCollectables = m_GameObjects.equal_range(typeid(EndLevelCollectable));
	for (auto iter = endLevelCollectables.first; iter != endLevelCollectables.second; ++iter) {
		if (m_Collision(playerCharacter->second->GetComponent<TransformComponent>()->m_Position, iter->second->GetComponent<TransformComponent>()->m_Position, glm::vec3(7.5f, 7.5f, 7.5f), glm::vec3(10.0f, 10.0f, 10.0f))) {
			iter->second->OnMessage("NextScene");
			return;
		}
	}

	for (auto gameObject : m_GameObjects) {
		gameObject.second->OnUpdate(p_DeltaTime);
	}

	RenderEngineInstance.SetGameObjects(&m_GameObjects);
	RenderEngineInstance.Update(p_DeltaTime);
}

void Scene::DisplayUnsuccessfullyLoadedModels(std::shared_ptr<IEngineCore> p_EngineInterface) {
	std::vector<std::string> unsuccessfullyLoadedModels = ModelManager::GetInstance().GetUnsuccessfullyLoadedModels();
	glm::vec2 windowPosition(0.005f, 0.9f);
	float textScale = 0.45f;
	float gapBetweenTextRow = 0.041f;

	if(unsuccessfullyLoadedModels.size() > 0)
		RenderEngineInstance.RenderText("Couldn't load these models:", windowPosition.x, windowPosition.y, textScale, glm::vec3(0.625f, 0.05f, 0.6f));
	for (const auto &unsuccessfulModel : unsuccessfullyLoadedModels) {
		windowPosition.y -= gapBetweenTextRow;
		RenderEngineInstance.RenderText(unsuccessfulModel, windowPosition.x, windowPosition.y, textScale, glm::vec3(0.6f, 0.05f, 0.6f));
	}
}

std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> Scene::GetObjectsRequiringInput() const {
	return m_ObjectsRequiringInput;
}

std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> Scene::GetObjects() const
{
	return m_GameObjects;
}
