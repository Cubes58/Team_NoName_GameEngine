#include "EnemyTower.h"

#include "PolymorphicInstanceManager.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "HealthComponent.h"
#include "AttackComponent.h"

EnemyTower::EnemyTower(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
	float p_AttackDamage, float p_AttackDistance, float p_AttackSpeed, float p_Health) 
	: EnemyTower(p_ModelName, p_Position, p_Orientation, p_Scale, p_AttackDamage, p_AttackDistance, p_AttackSpeed, p_Health, p_Health) {}	// Use p_Health as the max health.

EnemyTower::EnemyTower(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
	float p_AttackDamage, float p_AttackDistance, float p_AttackSpeed, float p_Health, float p_MaxHealth) {

	unsigned int index = 0;
	ModelComponent modelComponent(p_ModelName);
	PolymorphicInstanceManager::Instance().m_ModelComponents.PushBack(modelComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_ModelComponents.At(index));

	TransformComponent transformComponent(p_Position, p_Orientation, p_Scale);
	PolymorphicInstanceManager::Instance().m_TransformComponents.PushBack(transformComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_TransformComponents.At(index));

	HealthComponent healthComponent(p_Health);
	PolymorphicInstanceManager::Instance().m_HealthComponents.PushBack(healthComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_HealthComponents.At(index));

	AttackComponent attackComponent(p_AttackDamage, p_AttackDistance, p_AttackSpeed);
	PolymorphicInstanceManager::Instance().m_AttackComponents.PushBack(attackComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_AttackComponents.At(index));
}

void EnemyTower::OnUpdate(float p_DeltaTime) {
	GetComponent<AttackComponent>()->OnUpdate(p_DeltaTime);
}

void EnemyTower::OnMessage(const std::string &p_Message) {
	if (p_Message == "Attack") {
		GetComponent<AttackComponent>()->OnMessage(p_Message);
	}
}