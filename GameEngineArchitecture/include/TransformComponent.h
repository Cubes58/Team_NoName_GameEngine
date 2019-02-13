/**
@file TransformComponent.h
@brief A class that stores the properties necessary to move around the scene.
*/
#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#define ROTATION_SPEED 0.025f	//!< Default rotation speed.
#define MOVEMENT_SPEED 0.01f	//!< Default movement speed.
#define SCALE_SPEED 0.01f	//!< Default scale speed.

/*! \class TransformComponent
	\brief A class that stores the properties necessary to move around the scene.
*/

class TransformComponent : public Component {
public:
	glm::vec3 m_Position;	//!< Stores the position of the component (object it's attached to).
	glm::quat m_Orientation;	//!< Stores the orientation of the component (object it's attached to).
	glm::vec3 m_Scale;	//!< Stores the scale of the component (object it's attached to).

	/*!
		\brief Default constructor for the camera, it initialises the class' variables.
	*/
	TransformComponent() : m_Position(0), m_Orientation(1, 0, 0, 0), m_Scale(1.0f) {}
	/*!
		\brief Constructor for the TransformComponent, it initialises the class' variables.
		\param p_Position starting position of the object.
	*/
	TransformComponent(const glm::vec3 &p_Position) : m_Position(p_Position), m_Orientation(1, 0, 0, 0), m_Scale(1.0f) {}
	/*!
		\brief Constructor for the TransformComponent, it initialises the class' variables.
		\param p_Position starting position of the object.
		\param p_Orientation starting orientation of the object.
	*/
	TransformComponent(const glm::vec3 &p_Position, const glm::quat &p_Orientation) : m_Position(p_Position), m_Orientation(p_Orientation), m_Scale(1.0f) {}
	/*!
		\brief Constructor for the TransformComponent, it initialises the class' variables.
		\param p_Position starting position of the object.
		\param p_Orientation starting orientation of the object.
		\param p_Scale starting scale of the object.
	*/
	TransformComponent(const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale) : m_Position(p_Position), m_Orientation(p_Orientation), m_Scale(p_Scale) {}
	~TransformComponent() = default;	//!< Default destructor.

	/*!
		\brief Translates the object.
		\param p_TranslateVector the translate vector, used to move the object.
	*/
	void Translate(const glm::vec3 &p_TranslateVector) { 
		m_Position += p_TranslateVector; 
	}
	/*!
		\brief Translates the object.
		\param p_XAxis the X axis translation.
		\param p_YAxis the Y axis translation.
		\param p_ZAxis the Z axis translation.
	*/
	void Translate(float p_XAxis, float p_YAxis, float p_ZAxis) { 
		m_Position += glm::vec3(p_XAxis, p_YAxis, p_ZAxis); 
	}

	/*!
		\brief Rotates the object.
		\param p_Angle the rotation angle.
		\param p_Axis the axis to rotate around.
	*/
	void Rotate(float p_Angle, const glm::vec3 &p_Axis) { 
		m_Orientation *= glm::angleAxis(p_Angle, p_Axis * m_Orientation); 
	}
	/*!
		\brief Rotates the object.
		\param p_Angle the rotation angle.
		\param p_XAxis the X axis value to rotate around.
		\param p_YAxis the Y axis value to rotate around.
		\param p_ZAxis the Z axis value to rotate around.
	*/
	void Rotate(float p_Angle, float p_XAxis, float p_YAxis, float p_ZAxis) { 
		m_Orientation *= glm::angleAxis(p_Angle, glm::vec3(p_XAxis, p_YAxis, p_ZAxis) * m_Orientation); 
	}

	/*!
		\brief scales the object.
		\param p_ScaleVector the scale.
	*/
	void ScaleUp(const glm::vec3 &p_ScaleVector) { 
		m_Scale += p_ScaleVector; 
	}
	/*!
		\brief scales the object.
		\param p_XAxis the X axis scale value.
		\param p_YAxis the Y axis scale value.
		\param p_ZAxis the Z axis scale value.
	*/
	void ScaleUp(float p_XAxis, float p_YAxis, float p_ZAxis) { 
		m_Scale += glm::vec3(p_XAxis, p_YAxis, p_ZAxis); 
	}

	/*!
		\brief Rotates the object around the Y axis.
		\param p_Angle the rotation angle.
	*/
	void Yaw(float p_Angle) { 
		Rotate(p_Angle, 0.0f, 1.0f, 0.0f); 
	}
	/*!
		\brief Rotates the object around the X axis.
		\param p_Angle the rotation angle.
	*/
	void Pitch(float p_Angle) { 
		Rotate(p_Angle, 1.0f, 0.0f, 0.0f); 
	}
	/*!
		\brief Rotates the object around the Z axis.
		\param p_Angle the rotation angle.
	*/
	void Roll(float p_Angle) { 
		Rotate(p_Angle, 0.0f, 0.0f, 1.0f); 
	}

	/*!
		\brief Updates the Translate component.
		\param p_DeltaTime time passed.
	*/
	void OnUpdate(float p_DeltaTime) override {

	}

	/*!
		\brief Sends a message to the component.
		\param p_Message message being sent to the component.
	*/
	void OnMessage(const std::string &p_Message) override {
		if (p_Message == "positiveRoll") {
			Roll(ROTATION_SPEED);
		}
		else if (p_Message == "negativeRoll") {
			Roll(-ROTATION_SPEED);
		}
		else if (p_Message == "positivePitch") {
			Pitch(ROTATION_SPEED);
		}
		else if (p_Message == "negativePitch") {
			Pitch(-ROTATION_SPEED);
		}
		else if (p_Message == "scaleXUp") {
			ScaleUp(SCALE_SPEED, 0.0f, 0.0f);
		}
		else if (p_Message == "scaleXDown") {
			ScaleUp(-SCALE_SPEED, 0.0f, 0.0f);
		}
		else if (p_Message == "translateLeft") {
			Translate(glm::vec3(-MOVEMENT_SPEED, 0.0f, 0.0f));
		}
		else if (p_Message == "translateRight") {
			Translate(glm::vec3(MOVEMENT_SPEED, 0.0f, 0.0f));
		}
	}

	/*!
		\brief Gets the position of the Translate component.
		\return returns the position of the Translate component.
	*/
	const glm::vec3 &Position() const { 
		return m_Position; 
	}

	/*!
		\brief Gets the Orientation of the Translate component.
		\return returns the Orientation of the Translate component.
	*/
	const glm::quat &Orientation() const { 
		return m_Orientation; 
	}

	/*!
		\brief Gets the Scale of the Translate component.
		\return returns the Scale of the Translate component.
	*/
	const glm::vec3 &Scale() const { 
		return m_Scale; 
	}

	/*!
		\brief Gets the model matrix of the Translate component.
		\return returns the model matrix of the Translate component.
	*/
	glm::mat4 GetModelMatrix() {
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_Orientation);

		return transMatrix * rotMatrix * scaleMatrix;
	}
};