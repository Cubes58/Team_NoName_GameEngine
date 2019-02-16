#include "..\include\PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	m_InitialTime = std::chrono::system_clock::now();
}

void PhysicsEngine::Update(std::shared_ptr<Game> p_Game)
{
	m_CurrentTime = std::chrono::system_clock::now();
	m_ElapsedSeconds = m_CurrentTime - m_FrameStartTime;
	m_FrameStartTime = m_CurrentTime;
	//std::cout << "Frame start: " << (m_FrameStartTime - m_InitialTime).count() << "ns" << std::endl;
	//std::cout << "Elapsed seconds: " << m_ElapsedSeconds.count() << "s" << std::endl;

	if (m_ElapsedSeconds > m_SecondLimit) {
		m_ElapsedSeconds = m_SecondLimit;
		//std::cout << "Elapsed time to high" << std::endl;
	}
	std::cout << "engine fps: " << 1 / m_ElapsedSeconds.count() << "s" << std::endl;
	m_AccumulatedSeconds += m_ElapsedSeconds;
	//std::cout << "Accumulated seconds: " << m_AccumulatedSeconds.count() << "s" << std::endl;
	while (m_AccumulatedSeconds >= m_FramePeriod) {
		p_Game->Update(m_FramePeriod.count());
		std::cout << "physics fixed fps: " << 1 / m_FramePeriod.count() << std::endl;
		std::cout << "physics frame period: " << m_AccumulatedSeconds.count() << "s" << std::endl;
		m_AccumulatedSeconds -= m_FramePeriod;
	}
}