/**
@file StaticEnvironmentObject.h
@brief A class that stores the properties necessary to create a static scene object.
*/
#pragma once

#include <string>

#include <glm/glm.hpp>

#include "GameObject.h"

/*! \class StaticEnvironmentObject
	\brief A class that stores the properties necessary to create a static scene object.
*/

class StaticEnvironmentObject : public GameObject {
public:
	/*!
		\brief Constructor.
		\param p_ModelName name of the model, used by the object.
		\param p_Position starting position of the object.
		\param p_Orientation starting orientation of the object
		\param p_Scale starting scale of the object.
	*/
	StaticEnvironmentObject(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale);
	~StaticEnvironmentObject() = default;	//!< Default destructor.

	/*!
		\brief Updates the object.
		\param p_DeltaTime time passed.
	*/
	void OnUpdate(float p_DeltaTime) override;
	/*!
		\brief Sends a message to the object.
		\param p_Message message being sent to the object.
	*/
	void OnMessage(const std::string &p_Message) override;
};