#include "Random.h"

//make the instance a nullptr intially
Random * Random::m_Instance = nullptr;

Random::Random()
{
	std::random_device rd;
	m_MT = std::mt19937(rd());
}

Random::~Random()
{
	delete m_Instance;
	m_Instance = nullptr;
}

Random * Random::instance()
{
	if (m_Instance == nullptr) {
		m_Instance = new Random();
	}
	return m_Instance;
}

float Random::getRand(float p_Min, float p_Max)
{
	std::uniform_real_distribution<float> l_UniformDistrib(p_Min, p_Max);
	return l_UniformDistrib(m_MT);
}