#include "PhysicsEngine.h"

#include "BodyComponent.h"
#include "TransformComponent.h"

PhysicsEngine::PhysicsEngine(std::shared_ptr<Game> p_Game)
{
	m_InitialTime = std::chrono::system_clock::now();
	m_Game = p_Game;
	//m_QuadTree = nullptr;
}

void PhysicsEngine::GiveObjects(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> p_Objects)
{
	m_Objects = p_Objects;
	/*
	if (m_QuadTree != nullptr) {
		delete m_QuadTree;
		m_QuadTree = nullptr;
	}
	m_QuadTree = new QuadTree(0, BoundingBox(-100, -100, 200, 250));
	*/
}

void PhysicsEngine::Update()
{
	m_CurrentTime = std::chrono::system_clock::now();
	m_ElapsedSeconds = (m_CurrentTime - m_FrameStartTime).count();
	m_FrameStartTime = m_CurrentTime;

	if (m_ElapsedSeconds > m_SecondLimit) {
		m_ElapsedSeconds = m_SecondLimit;
	}

	//std::cout << "engine fps: " << 1 / m_ElapsedSeconds << "s" << std::endl;
	m_AccumulatedSeconds += m_ElapsedSeconds;
	while (m_AccumulatedSeconds >= m_FramePeriod) {
		m_Game->Update(m_FramePeriod);
		//std::cout << "Frame ratio (R/P): " << m_FramePeriod / m_ElapsedSeconds << std::endl;
		//std::cout << "physics fixed fps: " << 1 / m_FramePeriod << std::endl;
		//std::cout << "physics frame period: " << m_AccumulatedSeconds << "s" << std::endl;
		m_AccumulatedSeconds -= m_FramePeriod;

		PhysicsFrame();
	}
}

void PhysicsEngine::PhysicsFrame()
{

	CollisionChecks();

	/*
	if (m_Objects.size() > 0) {
		m_QuadTree->Clear();
		for (auto l_Itr = m_Objects.begin(); l_Itr != m_Objects.end(); ++l_Itr) {
			m_QuadTree->Insert(l_Itr->second);
		}

		for (auto l_Object : m_Objects) {
			std::vector<std::shared_ptr<GameObject>> l_NearbyObjects;
			glm::vec3 l_Pos = l_Object.second->GetComponent<TransformComponent>()->Position();
			m_QuadTree->Retrieve(l_Pos.x, l_Pos.y, l_NearbyObjects);

		}
	}
	*/
}

void PhysicsEngine::CollisionChecks()
{
	for (auto l_ItrA = m_Objects.begin(); l_ItrA != m_Objects.end(); ++l_ItrA) {
		//std::cout << "loop" << std::endl;
		glm::vec3 l_PrevPos = glm::vec3(0.f,0.f,0.f);

		if (l_ItrA->second->GetComponent<BodyComponent>() != nullptr) {
			l_ItrA->second->GetComponent<BodyComponent>()->ApplyAcceleration(glm::vec3(0.0f, -0.5f, 0.0f));
		}

		if (l_ItrA->second->GetComponent<AABBComponent>() != nullptr) {
			l_PrevPos = l_ItrA->second->GetComponent<AABBComponent>()->GetPosition();
		}

		l_ItrA->second->OnUpdate(m_FramePeriod);
		
		if (l_ItrA->second->GetComponent<AABBComponent>() != nullptr) {
			
			glm::vec3 l_ACornerA = l_ItrA->second->GetComponent<AABBComponent>()->m_CornerA;
			glm::vec3 l_ACornerB = l_ItrA->second->GetComponent<AABBComponent>()->m_CornerB;
			for (auto l_ItrB = m_Objects.begin(); l_ItrB != m_Objects.end(); ++l_ItrB) {
				//std::cout << "loop2" << std::endl;
				if (l_ItrB->second->GetComponent<AABBComponent>() != nullptr and l_ItrA != l_ItrB) {
					glm::vec3 l_BCornerA = l_ItrB->second->GetComponent<AABBComponent>()->m_CornerA;
					glm::vec3 l_BCornerB = l_ItrB->second->GetComponent<AABBComponent>()->m_CornerB;
					//std::cout << "diff" << std::endl;
					if (m_CollisionChecker.AABBAABB(l_ACornerA, l_ACornerB, l_BCornerA, l_BCornerB)) {
						//std::cout << "col" << std::endl;
						//glm::vec3 test = l_ItrA->second->GetComponent<BodyComponent>()->m_Position;
						//std::cout << "prev: " << l_PrevPos.y << std::endl;
						//std::cout << "cur: " << test.y << std::endl;
						l_ItrA->second->GetComponent<AABBComponent>()->SetPosition(l_PrevPos);
						if (l_ItrA->second->GetComponent<TransformComponent>() != nullptr) {
							l_ItrA->second->GetComponent<TransformComponent>()->m_Position = l_PrevPos;
						}

						if (l_ItrA->second->GetComponent<BodyComponent>() != nullptr) {
							l_ItrA->second->GetComponent<BodyComponent>()->m_Position = l_PrevPos;
							l_ItrA->second->GetComponent<BodyComponent>()->ApplyAcceleration(glm::vec3(0.0f, 0.5f, 0.0f));

						}
						//std::cout << "new: " << l_ItrA->second->GetComponent<BodyComponent>()->m_Position.y << std::endl;

					}
				}
			}
		}
	}
}
