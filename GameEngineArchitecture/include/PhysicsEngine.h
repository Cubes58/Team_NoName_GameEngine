#pragma once
#include "Game.h"
#include <chrono>
#include <iostream>

class PhysicsEngine {
private:
	unsigned int m_FPS = 60;
	std::chrono::duration<double> m_FramePeriod = std::chrono::duration<double>(1.0f / m_FPS);
	std::chrono::time_point<std::chrono::system_clock> m_FrameStartTime, m_InitialTime, m_CurrentTime;
	std::chrono::duration<double> m_ElapsedSeconds;
	std::chrono::duration<double> m_SecondLimit = std::chrono::duration<double>(0.25);
	std::chrono::duration<double> m_AccumulatedSeconds;

public:
	PhysicsEngine();
	void Update(std::shared_ptr<Game> p_Game);
};
