#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include "CollisionChecker.h"

class AABBComponent : public Component {
private:
	CollisionChecker m_CollisionChecker;
public:
	glm::vec3 m_CornerA;
	glm::vec3 m_CornerB;
	glm::vec3 m_Size;
	glm::vec3 m_PrevPos;

	AABBComponent(glm::vec3 p_Pos, glm::vec3 p_Size) {
		m_CornerA.x = p_Pos.x - (p_Size.x / 2.0f);
		m_CornerA.y = p_Pos.y - (p_Size.y / 2.0f);
		m_CornerA.z = p_Pos.z - (p_Size.z / 2.0f);

		m_CornerB.x = p_Pos.x + (p_Size.x / 2.0f);
		m_CornerB.y = p_Pos.y + (p_Size.y / 2.0f);
		m_CornerB.z = p_Pos.z + (p_Size.z / 2.0f);

		m_Size = p_Size;
	}

	void SetPosition(glm::vec3 p_Pos) {
		m_CornerA.x = p_Pos.x - (m_Size.x / 2.0f);
		m_CornerA.y = p_Pos.y - (m_Size.y / 2.0f);
		m_CornerA.z = p_Pos.z - (m_Size.z / 2.0f);

		m_CornerB.x = p_Pos.x + (m_Size.x / 2.0f);
		m_CornerB.y = p_Pos.y + (m_Size.y / 2.0f);
		m_CornerB.z = p_Pos.z + (m_Size.z / 2.0f);

		m_PrevPos = p_Pos;
	}

	bool Check(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> p_Objects) {
		for (auto l_Itr = p_Objects.begin(); l_Itr != p_Objects.end(); ++l_Itr) {
			if (l_Itr->second->GetComponent<AABBComponent>() != nullptr) {
				glm::vec3 l_CornerA = l_Itr->second->GetComponent<AABBComponent>()->m_CornerA;
				glm::vec3 l_CornerA = l_Itr->second->GetComponent<AABBComponent>()->m_CornerB;
				if (m_CollisionChecker.AABBAABB(m_CornerA, m_CornerB, m_CornerA, m_CornerB)) {
					return true;
				}
			}
			//Other checks
		}
		return false;
	}

	//TODO
	void Resolve() {
		SetPosition(m_PrevPos);
	}

	void OnUpdate(float p_DeltaTime) override {

	}

	void OnMessage(const std::string &p_Message) override {

	}
};
