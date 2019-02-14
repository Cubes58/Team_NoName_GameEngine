#include "..\include\PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	m_InitialTime = std::chrono::high_resolution_clock::now();
}

void PhysicsEngine::Update(std::shared_ptr<Game> p_Game)
{
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	m_ElapsedSeconds = m_CurrentTime - m_FrameStartTime;
	m_FrameStartTime = m_CurrentTime;
	//std::cout << "###Frame start: " << (m_FrameStartTime - m_InitialTime).count() << "ns" << std::endl;
	std::cout << "Engine !! elapsed seconds: " << m_ElapsedSeconds.count() << "s" << std::endl;

	if (m_ElapsedSeconds > m_SecondLimit) {
		m_ElapsedSeconds = m_SecondLimit;
		std::cout << "##Previous frame time exceeded time limit" << std::endl;
	}

	//m_AccumulatedSeconds += m_ElapsedSeconds;
	//std::cout << "##Accumulated seconds: " << m_AccumulatedSeconds.count() << std::endl;
	//std::cout << "##Fixed physics frame period: " << m_FramePeriod.count() << std::endl;
	//while (m_AccumulatedSeconds >= m_FramePeriod) {
		p_Game->Update(m_FramePeriod.count());
	//	m_AccumulatedSeconds -= m_FramePeriod;
		//std::cout << "#physics fps: " << 1.0f / m_AccumulatedSeconds.count() << std::endl;
		//std::cout << "#physics frame period: " << m_AccumulatedSeconds.count() << "s" << std::endl;
	//}
}
