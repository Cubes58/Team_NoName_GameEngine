#pragma once

#include "Component.h"
#include <glm/glm.hpp>

enum class BodyType : unsigned int {
	STATIC,
	KINEMATIC,
	DYNAMIC
};

class BodyComponent : public Component {
private:
	BodyType m_BodyType = BodyType::KINEMATIC;
	void Integrate(float p_DeltaTime) {
		m_Velocity += m_Acceleration * p_DeltaTime;
		m_Position += m_Velocity * p_DeltaTime;
	}
protected:
	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;

	float m_Mass;
	float m_Restitution;
	float m_DragCo;
	float m_FrictionCo;
public:
	BodyComponent(glm::vec3 p_Pos, glm::vec3 p_Vel, glm::vec3 p_Accel, float p_Mass, float p_Rest, float p_DragCo, float p_FrictionCo) {
		m_Position = p_Pos;
		m_Velocity = p_Vel;
		m_Acceleration = p_Accel;

		m_Mass = p_Mass;
		m_Restitution = p_Rest;
		m_DragCo = p_DragCo;
		m_FrictionCo = p_FrictionCo;
	}
	void ApplyForce(glm::vec3 p_Force) {
		m_Acceleration += (p_Force / m_Mass);
	}

	void OnUpdate(float p_DeltaTime) override {
		switch (m_BodyType) {
		case BodyType::STATIC:
			// sad faic
			break;
		case BodyType::KINEMATIC:
			Integrate(p_DeltaTime);
			break;
		case BodyType::DYNAMIC:
			Integrate(p_DeltaTime);
			m_Acceleration = glm::vec3(0.f, 0.f, 0.f);
			break;
		default:

			break;
		}
	}

	void OnMessage(const std::string &p_Message) override {

	}
};
