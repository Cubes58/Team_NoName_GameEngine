/**
@file PlayerCharacter.h
@brief A class that stores the properties necessary to create a player character - the object responds to the player's input, and the camera follows the object around the scene.
*/
#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "GameObject.h"
#include "CameraViewState.h"

/*! \class PlayerCharacter
	\brief A class that stores the properties necessary to create a player character - the object responds to the player's input, and the camera follows the object around the scene.
*/

class PlayerCharacter : public GameObject {
private:	
	CameraViewState m_CameraState;	//!< Stores the view state of the camera.
	float m_MouseSensitivity = 0.2f;	//!< Stores the sensitivity of the mouse rotation.
	bool m_InverseRotation = false;	//!< Stores whether the player's camera rotation should be inversed.

	float m_PlayerMovementSpeed;	//!< Stores the player's movement speed.
	float m_PlayerRotationSpeed;	//!< Stores the player's rotation speed.

	// Used to reset the player, when they die.
	glm::vec3 m_OriginalPosition;	//!< Stores the player's starting position, so it can be reset.
	glm::quat m_OriginalOrientation;	//!< Stores the player's starting orientation, so it can be reset.

public:
	/*!
		\brief Constructor.
		\param p_ModelName name of the model, used by the object.
		\param p_Position starting position of the object.
		\param p_Orientation starting orientation of the object
		\param p_Scale starting scale of the object.
		\param p_Health starting health of the object.
		\param p_PlayerMovementSpeed starting movement speed of the object.
		\param p_PlayerRotationSpeed starting rotation speed of the object.
	*/
	PlayerCharacter(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, 
		const glm::vec3 &p_Scale, float p_Health = 100.0f, float p_PlayerMovementSpeed = 0.35f, float p_PlayerRotationSpeed = 0.045f);
	~PlayerCharacter() = default;	//!< Default destructor.

	/*!
		\brief Updates the game object.
		\param p_DeltaTime time passed.
	*/
	void OnUpdate(float p_DeltaTime) override;
	/*!
		\brief Sends a message to the object.
		\param p_Message message being sent to the object.
	*/
	void OnMessage(const std::string &p_Message) override;

	/*!
		\brief Uses the cursor input to rotate the camera component and translate component.
		\param p_DeltaMouseXPosition the difference between the X axis mouse position and the previous X axis mouse position.
		\param p_DeltaMouseYPosition the difference between the Y axis mouse position and the previous Y axis mouse position.
	*/
	void HandleCursorInput(double p_DeltaMouseXPosition, double p_DeltaMouseYPosition);

	/*!
		\brief Gets the CameraPerspective of the object.
		\return returns the CameraPerspective of the object.
	*/
	inline CameraViewState CameraPerspective() const {
		return m_CameraState;
	}

	/*!
		\brief Gets the object's mouse sensitivity.
		\return returns the object's mouse sensitivity.
	*/
	inline float GetMouseSensitivity() const {
		return m_MouseSensitivity;
	}

	/*!
		\brief Gets the inverse rotation of the object.
		\return returns the inverse rotation of the object.
	*/
	inline bool InverseRotation() const {
		return m_InverseRotation;
	}
};