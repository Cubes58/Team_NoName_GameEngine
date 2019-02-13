/**
@file Collision.h
@brief A class that handles collisions.
*/
#pragma once

#include <cmath>

#include <glm/glm.hpp>

/*! \class Collision
	\brief A class that handles collisions.
*/
class Collision {
public:
	Collision() = default;	//!< Default constructor.
	~Collision() = default;	//!< Default destructor.

	/*!
		\brief Detects a collision between two AABB objects.
		\param p_EntityOnePosition entity one's position.
		\param p_EntityTwoPosition entity two's position.
		\param p_EntityOneSize entity one's size.
		\param p_EntityTwoSize entity two's size.
		\return Returns true if a collision has happened, false otherwise.
	*/
	bool operator()(const glm::vec3 &p_EntityOnePosition, const glm::vec3 &p_EntityTwoPosition, const glm::vec3 &p_EntityOneSize, const glm::vec3 &p_EntityTwoSize) {
		glm::vec2 entityOneXAxisLine(p_EntityOnePosition.x - p_EntityOneSize.x, p_EntityOnePosition.x + p_EntityOneSize.x);
		glm::vec2 entityOneYAxisLine(p_EntityOnePosition.y - p_EntityOneSize.y, p_EntityOnePosition.y + p_EntityOneSize.y);
		glm::vec2 entityOneZAxisLine(p_EntityOnePosition.z - p_EntityOneSize.z, p_EntityOnePosition.z + p_EntityOneSize.z);

		glm::vec2 entityTwoXAxisLine(p_EntityTwoPosition.x - p_EntityTwoSize.x, p_EntityTwoPosition.x + p_EntityTwoSize.x);
		glm::vec2 entityTwoYAxisLine(p_EntityTwoPosition.y - p_EntityTwoSize.y, p_EntityTwoPosition.y + p_EntityTwoSize.y);
		glm::vec2 entityTwoZAxisLine(p_EntityTwoPosition.z - p_EntityTwoSize.z, p_EntityTwoPosition.z + p_EntityTwoSize.z);

		// AABB collision check.
		if (entityOneXAxisLine.x >= entityTwoXAxisLine.x && entityOneXAxisLine.y <= entityTwoXAxisLine.y 
			&& entityOneYAxisLine.x >= entityTwoYAxisLine.x && entityOneYAxisLine.y <= entityTwoYAxisLine.y
			&& entityOneZAxisLine.x >= entityTwoZAxisLine.x && entityOneZAxisLine.y <= entityTwoZAxisLine.y) {
			return true;
		}

		return false;
	}
};