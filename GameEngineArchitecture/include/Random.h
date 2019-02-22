/*!
@file Random.h
*/

#pragma once

#include <iostream>
#include <random>

/*! \class Random
\brief Singleton class that holds random number engines
*/

class Random {
private:
	Random();	//!< Private constructor
	static Random * m_Instance;	//!< Static pointer to the singleton random object
	std::mt19937 m_MT; //!< mersenne twister random number engine
	~Random();	//!< Private destructor
public:
	static Random * instance();	//!< Returns a pointer to the static texture loader
								/*!
								* Get random number between a min and max inclusive
								* @param[in] p_Min Minimum value
								* @param[in] p_Max Maximum value
								*/
	float getRand(float p_Min, float p_Max);
	Random(Random const&) = delete; //!< prevents the creation of multiple texture loaders
	Random& operator=(Random const&) = delete; //!< prevents the creation of multiple texture loaders
};