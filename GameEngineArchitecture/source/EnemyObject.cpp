#include "EnemyObject.h"

#include "PolymorphicInstanceManager.h"

#include "ModelComponent.h"
#include "BodyComponent.h"
#include "AABBComponent.h"
#include "TransformComponent.h"
#include "Graph.h"
#include "GraphNode.h"


EnemyObject::EnemyObject(const std::string &p_ModelName, const glm::vec3& p_Pos,
	const glm::vec3& p_AABBSize, const glm::quat &p_ModelOrientation, const glm::vec3& p_ModelScale,
	float p_Mass, const BodyType &p_BodyType, float p_Rest, float p_DragCo, float p_FrictionCo) {

	unsigned int index = 0;
	ModelComponent modelComponent(p_ModelName);
	PolymorphicInstanceManager::Instance().m_ModelComponents.PushBack(modelComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_ModelComponents.At(index));

	TransformComponent transformComponent(p_Pos, p_ModelOrientation, p_ModelScale);
	PolymorphicInstanceManager::Instance().m_TransformComponents.PushBack(transformComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_TransformComponents.At(index));

	m_Graph = new Graph(glm::vec3(-24.f, 0.f, -24.f), 1.f, glm::vec2(50u, 50u));
	counter = 0;
}

void EnemyObject::ApplyForce(glm::vec3 p_Force)
{
}

void EnemyObject::SetPosition(glm::vec3 p_Pos)
{
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
	TransformComponent *transform = GetComponent<TransformComponent>();
	transform->Translate(m_TranslationVector * p_DeltaTime * inverse(transform->m_Orientation));

	if (!IsPathEmpty()) {

		if (m_List.front() != nullptr) {

			counter++;
			if (counter == 220)
			{
				transform->m_Position = m_List.front()->GetNodesPos();
				m_List.pop_front();
				counter = 0;
			}

		}

	}
	else {

	}

}

void EnemyObject::OnMessage(const std::string &p_Message) {
	
}
