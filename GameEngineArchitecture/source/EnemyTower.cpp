#include "EnemyTower.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "HealthComponent.h"
#include "AttackComponent.h"

EnemyTower::EnemyTower(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
	float p_AttackDamage, float p_AttackDistance, float p_AttackSpeed, float p_Health) 
	: EnemyTower(p_ModelName, p_Position, p_Orientation, p_Scale, p_AttackDamage, p_AttackDistance, p_AttackSpeed, p_Health, p_Health) {}	// Use p_Health as the max health.

EnemyTower::EnemyTower(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
	float p_AttackDamage, float p_AttackDistance, float p_AttackSpeed, float p_Health, float p_MaxHealth) {
	AddComponent(std::make_shared<ModelComponent>(p_ModelName));
	AddComponent(std::make_shared<TransformComponent>(p_Position, p_Orientation, p_Scale));
	AddComponent(std::make_shared<HealthComponent>(p_Health, p_MaxHealth));
	AddComponent(std::make_shared<AttackComponent>(p_AttackDamage, p_AttackDistance, p_AttackSpeed));
}

void EnemyTower::OnUpdate(float p_DeltaTime) {
	GetComponent<AttackComponent>()->OnUpdate(p_DeltaTime);
}

void EnemyTower::OnMessage(const std::string &p_Message) {
	if (p_Message == "Attack") {
		GetComponent<AttackComponent>()->OnMessage(p_Message);
	}
}