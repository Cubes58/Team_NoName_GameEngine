/**
@file MovementComponent.h
@brief A class that stores the properties necessary for an entity to move around.
*/
#pragma once

#include <random>

#include "Component.h"

/*! \class MovementComponent
	\brief A class that stores the properties necessary for an entity to move around.
*/

static constexpr float s_DurationToSwitchAcceleration = 12.5f; //!< Constant, duration of when to switch the acceleration value[s].

class MovementComponent : public Component {
private:
	glm::vec3 m_Acceleration;	//!< The acceleration values, for each axis.
	glm::vec3 m_Velocity;	//!< The velocity values, for each axis.
	float m_TimePassed = 0;	//!< The time passed.
	std::random_device m_RandomDevice;	//!< Random device.
	std::default_random_engine m_Generator;	//!< Random number generator.

public:
	/*!
		\brief Constructor.
		\param p_Acceleration the starting acceleration of the entity.
		\param p_Velocity the starting velocity of the entity.
	*/
	MovementComponent(const glm::vec3 &p_Acceleration, const glm::vec3 &p_Velocity) : m_Acceleration(p_Acceleration), m_Velocity(p_Velocity), m_Generator(m_RandomDevice()) {}
	~MovementComponent() = default;	//!< Default destructor.

	/*!
		\brief Update the health component.
		\param p_DeltaTime the time passed.
	*/
	void OnUpdate(float p_DeltaTime) override {
		m_TimePassed += p_DeltaTime;
		if (m_TimePassed >= s_DurationToSwitchAcceleration) {
			OnMessage("ChangeAcceleration");
		}

		m_Velocity += m_Acceleration * p_DeltaTime;
	}

	/*!
		\brief Sends a message to the component.
		\param p_Message the message being sent to the component.
	*/
	void OnMessage(const std::string &p_Message) override {
		if (p_Message == "ChangeAcceleration") {
			std::uniform_real_distribution<float> distribution((float)-1.0f, (float)1.0f);
			m_Acceleration.x = distribution(m_Generator);
			m_Acceleration.z = distribution(m_Generator);
		}
	}

	/*!
		\brief Returns the acceleration of the component.
	*/
	const glm::vec3 &Acceleration() const {
		return m_Acceleration;
	}
	/*!
		\brief Returns the velocity of the component.
	*/
	const glm::vec3 &Velocity() const {
		return m_Velocity;
	}
};