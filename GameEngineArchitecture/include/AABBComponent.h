#pragma once

#include <glm/glm.hpp>

#include "Component.h"
#include "CollisionChecker.h"

class AABBComponent : public Component {
private:
	CollisionChecker m_CollisionChecker;
public:
	glm::vec3 m_CornerA;
	glm::vec3 m_CornerB;
	glm::vec3 m_Size;
	glm::vec3 m_Pos;

	AABBComponent(glm::vec3 p_Pos, glm::vec3 p_Size) {
		m_CornerA.x = p_Pos.x - (p_Size.x / 2.0f);
		m_CornerA.y = p_Pos.y - (p_Size.y / 2.0f);
		m_CornerA.z = p_Pos.z - (p_Size.z / 2.0f);

		m_CornerB.x = p_Pos.x + (p_Size.x / 2.0f);
		m_CornerB.y = p_Pos.y + (p_Size.y / 2.0f);
		m_CornerB.z = p_Pos.z + (p_Size.z / 2.0f);

		m_Size = p_Size;
		m_Pos = p_Pos;
	}

	void SetPosition(glm::vec3 p_Pos) {
		m_CornerA.x = p_Pos.x - (m_Size.x / 2.0f);
		m_CornerA.y = p_Pos.y - (m_Size.y / 2.0f);
		m_CornerA.z = p_Pos.z - (m_Size.z / 2.0f);

		m_CornerB.x = p_Pos.x + (m_Size.x / 2.0f);
		m_CornerB.y = p_Pos.y + (m_Size.y / 2.0f);
		m_CornerB.z = p_Pos.z + (m_Size.z / 2.0f);

		m_Pos = p_Pos;
	}

	glm::vec3 GetPosition() {
		return m_Pos;
	}

	void OnUpdate(float p_DeltaTime) override {

	}

	void OnMessage(const std::string &p_Message) override {

	}
};
