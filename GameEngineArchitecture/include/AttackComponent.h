/**
@file AttackComponent.h
@brief A class that stores the properties necessary for an entity to attack.
*/
#pragma once

#include "Component.h"

/*! \class AttackComponent
	\brief A class that stores the properties necessary for an entity to attack.
*/

class AttackComponent : public Component {
private:
	float m_AttackDamage;	//!< The damage an entity can do with an attack.
	float m_AttackDistance;	//!< How far the entity's attack goes.
	float m_AttackSpeed;	//!< The rate the entity can attack.
	float m_TimePassedSinceLastAttack;	//!< The time passed since the entity last attacked.
	bool m_CanAttack = false;	//!< Stores whether the entity can attack.

public:
	/*!
		\brief Constructor.
		\param p_AttackDamage the damage the entity can do.
		\param p_AttackDistance how far the entity's attack goes.
		\param p_AttackSpeed the rate the entity can attack.
	*/
	AttackComponent(float p_AttackDamage, float p_AttackDistance, float p_AttackSpeed) 
		: m_AttackDamage(p_AttackDamage), m_AttackDistance(p_AttackDistance), m_AttackSpeed(p_AttackSpeed), m_TimePassedSinceLastAttack(m_AttackSpeed) {}
	~AttackComponent() = default; //!< Default destructor.

	/*!
		\brief Update the attack component.
		\param p_DeltaTime the time passed.
	*/
	void OnUpdate(float p_DeltaTime) override {
		m_TimePassedSinceLastAttack += p_DeltaTime;
		m_TimePassedSinceLastAttack >= m_AttackSpeed ? m_CanAttack = true : m_CanAttack = false;
	}

	/*!
		\brief Sends a message to the component.
		\param p_Message the message being sent to the component.
	*/
	void OnMessage(const std::string &p_Message) override {
		if (p_Message == "Attack") {
			m_TimePassedSinceLastAttack = 0;
			m_CanAttack = false;
		}
	}

	/*!
		\brief Returns the attack distance, of the entity.
	*/
	float GetAttackDistance() const {
		return m_AttackDistance;
	}

	/*!
		\brief Returns whether the entity can attack.
	*/
	bool CanAttack() const {
		return m_CanAttack;
	}
};