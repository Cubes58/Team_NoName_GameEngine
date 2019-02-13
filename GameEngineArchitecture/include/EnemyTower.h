/**
@file EnemyTower.h
@brief A class that stores the properties necessary to create a tower object that attacks the player.
*/
#pragma once

#include <string>

#include <glm/glm.hpp>

#include "GameObject.h"

/*! \class EnemyTower
	\brief A class that stores the properties necessary to create a tower object that attacks the player.
*/

class EnemyTower : public GameObject {
public:
	/*!
		\brief Constructor.
		\param p_ModelName name of the model, used by the object.
		\param p_Position starting position of the object.
		\param p_Orientation starting orientation of the object
		\param p_Scale starting scale of the object.
		\param p_AttackDamage starting damage of the object.
		\param p_AttackDistance attack distance of the object.
		\param p_AttackSpeed starting attack speed of the object.
		\param p_Health starting health of the object.
	*/
	EnemyTower(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
		float p_AttackDamage, float p_AttackDistance, float p_AttackSpeed, float p_Health);
	/*!
		\brief Constructor.
		\param p_ModelName name of the model, used by the object.
		\param p_Position starting position of the object.
		\param p_Orientation starting orientation of the object
		\param p_Scale starting scale of the object.
		\param p_AttackDamage starting damage of the object.
		\param p_AttackDistance attack distance of the object.
		\param p_AttackSpeed starting attack speed of the object.
		\param p_Health starting health of the object.
		\param p_MaxHealth max health of the object.
	*/
	EnemyTower(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, 
		float p_AttackDamage, float p_AttackDistance, float p_AttackSpeed, float p_Health, float p_MaxHealth);
	~EnemyTower() = default;	//!< Default destructor.

	/*!
		\brief Updates the object.
		\param p_DeltaTime time passed.
	*/
	void OnUpdate(float p_DeltaTime) override;
	/*!
		\brief Sends a message to the object.
		\param p_Message message being sent to the object.
	*/
	void OnMessage(const std::string &p_Message) override;
};