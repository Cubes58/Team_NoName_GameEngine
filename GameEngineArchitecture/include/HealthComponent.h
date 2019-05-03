/**
@file HealthComponent.h
@brief A class that stores the properties necessary for an entity to have health.
*/
#pragma once

#include "Component.h"

/*! \class HealthComponent
	\brief A class that stores the properties necessary for an entity to have health.
*/

constexpr int ENEMY_TOWER_DAMAGE = 20; //!< The damage taken by being hit by the tower.

class HealthComponent : public Component {
private:
	float m_Health;	//!< The current health of the entity.
	float m_MaxHealth;	//!< The maximum health of the entity.
	bool m_HealthBelowZero = false;	//!< Stores whether the entity's health is below zero.

public:
	/*!
		\brief Constructor.
		\param p_Health the starting health of the entity, this will also be used as the max health.
	*/
	HealthComponent(float p_Health) : HealthComponent(p_Health, p_Health) {}
	/*!
		\brief Constructor.
		\param p_Health the starting health of the entity.
		\param p_MaxHealth the max health of the entity.
	*/
	HealthComponent(float p_Health, float p_MaxHealth) : m_Health(p_Health), m_MaxHealth(p_MaxHealth) {}
	~HealthComponent() = default;	//!< Default destructor.

	/*!
		\brief Update the health component.
		\param p_DeltaTime the time passed.
	*/
	void OnUpdate(float p_DeltaTime) override {
		
	}

	/*!
		\brief Sends a message to the component.
		\param p_Message the message being sent to the component.
	*/
	void OnMessage(const std::string &p_Message) override {
		if (p_Message == "HitByEnemyTower") {
			m_Health -= ENEMY_TOWER_DAMAGE;
		}
		else if (p_Message == "Reset") {
			m_Health = m_MaxHealth;
			m_HealthBelowZero = false;
		}

		m_Health <= 0 ? m_HealthBelowZero = true : m_HealthBelowZero = false;
	}

	/*!
		\brief Returns the entity's current health.
	*/
	float GetHealth() const {
		return m_Health;
	}

	/*!
		\brief Returns whether the entity's health is below zero. True if it is.
	*/
	bool IsHealthBelowZero() const {
		return m_HealthBelowZero;
	}
};