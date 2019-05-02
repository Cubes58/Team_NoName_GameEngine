#include "EnemyObject.h"

#include "ModelComponent.h"
#include "BodyComponent.h"
#include "AABBComponent.h"
#include "TransformComponent.h"
#include "Graph.h"
#include "GraphNode.h"


EnemyObject::EnemyObject(Graph * p_Graph, const std::string &p_ModelName, const glm::vec3& p_Pos,
	const glm::vec3& p_AABBSize, const glm::quat &p_ModelOrientation, const glm::vec3& p_ModelScale,
	float p_Mass, const BodyType &p_BodyType, float p_Rest, float p_DragCo, float p_FrictionCo) {

	AddComponent(std::make_shared<ModelComponent>(p_ModelName));
	AddComponent(std::make_shared<BodyComponent>(p_Pos, p_Mass, p_Rest, p_DragCo, p_FrictionCo));
	AddComponent(std::make_shared<AABBComponent>(p_Pos, p_AABBSize));
	AddComponent(std::make_shared<TransformComponent>(p_Pos, p_ModelOrientation, p_ModelScale));

	GetComponent<BodyComponent>()->SetType(p_BodyType);

	m_Graph = p_Graph;
}

void EnemyObject::ApplyForce(glm::vec3 p_Force)
{
	GetComponent<BodyComponent>()->ApplyForce(p_Force);
}

void EnemyObject::SetPosition(glm::vec3 p_Pos)
{
	//GetComponent<BodyComponent>()->m_Position = p_Pos;
	GetComponent<AABBComponent>()->SetPosition(p_Pos);
	GetComponent<TransformComponent>()->m_Position = p_Pos;
}

void EnemyObject::CreatePath(float p_X, float p_Z) { //these floats will be used to get the goal Node
	m_List = m_Graph->GetThePath(GetComponent<TransformComponent>()->m_Position, glm::vec3(p_X, 0, p_Z));	
} //A * to create the path

bool EnemyObject::IsPathEmpty()
{
	if(m_List.empty()) 
	return true;

	return false;
}

void EnemyObject::OnUpdate(float p_DeltaTime)
{
	std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>();
	transform->Translate(m_TranslationVector * p_DeltaTime * inverse(transform->m_Orientation));

	if (!IsPathEmpty()) {

		if (m_List.front() != nullptr) {

			if (m_List.front()->GetNodesPos().x < GetComponent<TransformComponent>()->m_Position.x) {
				//m_TranslationVector.x = -m_PlayerMovementSpeed;
				m_TranslationVector.x = -0.6f;
			}
			else { m_TranslationVector.x = 0.6f; } //these two if's increment the player towards the goal

			if (m_List.front()->GetNodesPos().z < GetComponent<TransformComponent>()->m_Position.z) {
				//m_TranslationVector.x = -m_PlayerMovementSpeed;
				m_TranslationVector.z = -0.6f;
			}
			else { m_TranslationVector.z = 0.6f; }

			if (m_Graph->CalculateH(GetComponent<TransformComponent>()->m_Position, m_List.front()->GetNodesPos()) < 2.0f){
				m_List.pop_front(); //if it gets close enough to the node it will Pop it and move onto the next
			}

		}

	}

	GetComponent<BodyComponent>()->OnUpdate(p_DeltaTime);
	glm::vec3 l_Pos = GetComponent<BodyComponent>()->m_Position;
	SetPosition(l_Pos);

}

void EnemyObject::OnMessage(const std::string &p_Message) {
	if (p_Message == "static") {
		GetComponent<BodyComponent>()->SetType(BodyType::STATIC);
	}
	else if (p_Message == "kinematic") {
		GetComponent<BodyComponent>()->SetType(BodyType::KINEMATIC);
	}
	else if (p_Message == "dynamic") {
		GetComponent<BodyComponent>()->SetType(BodyType::DYNAMIC);
	}
}
