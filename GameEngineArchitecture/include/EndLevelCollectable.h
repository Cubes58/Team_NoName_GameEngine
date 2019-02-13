/**
@file EndLevelCollectable.h
@brief A class that stores the properties necessary to create an object that switches scenes, when collided with (checkpoint).
*/
#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "GameObject.h"

class DefaultGame;

/*! \class EndLevelCollectable
	\brief A class that stores the properties necessary to create an object that switches scenes, when collided with (checkpoint).
*/

class EndLevelCollectable : public GameObject {
private:
	std::weak_ptr<DefaultGame> m_Game;	//!< A weak pointer to the game object.

public:
	/*!
		\brief Constructor.
		\param p_ModelName name of the model, used by the object.
		\param p_Position starting position of the object.
		\param p_Orientation starting orientation of the object
		\param p_Scale starting scale of the object.
		\param p_Game a game pointer, so it can switch the scene, to the next one, when collided with.
	*/
	EndLevelCollectable(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, std::shared_ptr<DefaultGame> p_Game);
	~EndLevelCollectable() = default;	//!< Default destructor.

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