#pragma once
#include "Game.h"
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include "GameObject.h"
#include "QuadTree.h"

class PhysicsEngine {
private:
	std::chrono::time_point<std::chrono::system_clock> m_FrameStartTime, m_InitialTime, m_CurrentTime;
	unsigned int m_FPS = 60;
	double m_FramePeriod = std::chrono::duration<double>(1.0f / m_FPS).count();
	double m_ElapsedSeconds;
	double m_SecondLimit = std::chrono::duration<double>(0.25).count();
	double m_AccumulatedSeconds;

	std::shared_ptr<Game> m_Game;

	//QuadTree* m_QuadTree;
	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> m_Objects;
public:
	PhysicsEngine(std::shared_ptr<Game> p_Game);
	void GiveObjects(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> p_Objects);
	void Update();
	void PhysicsFrame();
};
