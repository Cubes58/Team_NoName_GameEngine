#include "..\include\PhysicsEngine.h"

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

	std::cout << "engine fps: " << 1 / m_ElapsedSeconds << "s" << std::endl;
	m_AccumulatedSeconds += m_ElapsedSeconds;
	while (m_AccumulatedSeconds >= m_FramePeriod) {
		m_Game->Update(m_FramePeriod);
		std::cout << "Frame ratio (R/P): " << m_FramePeriod / m_ElapsedSeconds << std::endl;
		std::cout << "physics fixed fps: " << 1 / m_FramePeriod << std::endl;
		std::cout << "physics frame period: " << m_AccumulatedSeconds << "s" << std::endl;
		m_AccumulatedSeconds -= m_FramePeriod;
	}
}

void PhysicsEngine::PhysicsFrame()
{
	for (auto l_Itr = m_Objects.begin(); l_Itr != m_Objects.end(); ++l_Itr) {
		if (l_Itr->second->GetComponent<AABBComponent>() != nullptr) {
			if (l_Itr->second->GetComponent<AABBComponent>()->Check(m_Objects)) {
				l_Itr->second->GetComponent<AABBComponent>()->Resolve();
			}
		}
		l_Itr->second->OnUpdate(m_FramePeriod);
	}

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
