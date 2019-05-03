#pragma once

#include <string>

#include <glm/glm.hpp>

#include "GameObject.h"
#include "BodyEnums.h"

class Graph;
class GraphNode;


class EnemyObject : public GameObject {
protected:

    glm::vec3 m_TranslationVector = glm::vec3(0, 0, 0);
	std::list<GraphNode*> m_List;
	Graph * m_Graph;
	int counter;
	

public:
	EnemyObject(const std::string &p_ModelName, const glm::vec3& p_Pos, const glm::vec3& p_AABBSize,
		const glm::quat &p_ModelOrientation, const glm::vec3& p_ModelScale, float p_Mass, const BodyType &p_BodyType,
		float p_Rest = 0.0f, float p_DragCo = 0.0f, float p_FrictionCo = 0.0f);
	~EnemyObject() = default;

	void ApplyForce(glm::vec3 p_Force);

	void SetPosition(glm::vec3 p_Pos);

	void CreatePath(float p_X, float p_Z);
	bool IsPathEmpty();

	void OnUpdate(float p_DeltaTime) override;
	void OnMessage(const std::string &p_Message) override;
};