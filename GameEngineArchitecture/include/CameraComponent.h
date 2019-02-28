/**
@file CameraComponent.h
@brief A class that stores the properties necessary for a camera component, to be added to an object.
*/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Component.h"

/*! \class CameraComponent
	\brief A class that stores the properties necessary for a camera component, to be added to an object.
*/

class CameraComponent : public Component {
public:
	glm::vec3 m_Position;	//!< Position of the camera.
	glm::quat m_Orientation;	//!< Orientation of the camera.
	float m_FieldOfView;	//!< Camera's Field of view.

	/*!
		\brief Default constructor for the camera, it initialises the class' variables.
	*/
	CameraComponent() : m_Position(0), m_Orientation(1, 0, 0, 0), m_FieldOfView(60.0f) {}
	
	/*!
		\brief Constructor for the camera, it initialises the class' variables.
		\param p_Position starting position of the camera.
	*/
	CameraComponent(const glm::vec3 &p_Position) : m_Position(p_Position), m_Orientation(1, 0, 0, 0), m_FieldOfView(60.0f) {}
	
	/*!
		\brief Constructor for the camera, it initialises the class' variables.
		\param p_Position starting position of the camera.
		\param p_Orientation starting orientation of the camera.
	*/
	CameraComponent(const glm::vec3 &p_Position, const glm::quat &p_Orientation) : m_Position(p_Position), m_Orientation(p_Orientation), m_FieldOfView(60.0f) {}
	~CameraComponent() = default;	//!< Default destructor.


	/*!
		\brief Updates the camera component.
		\param p_DeltaTime time passed.
	*/
	void OnUpdate(float p_DeltaTime) override {

	}

	/*!
		\brief Sends a message to the component.
		\param p_Message message being sent to the component.
	*/
	void OnMessage(const std::string &p_Message) override {

	}

	/*!
		\brief Translates the camera component.
		\param p_TranslateVector the translate vector, used to move the camera object.
	*/
	void Translate(const glm::vec3 &p_TranslateVector) {
		m_Position += p_TranslateVector * m_Orientation;
	}
	/*!
		\brief Translates the camera component.
		\param p_XAxis the X axis translation.
		\param p_YAxis the Y axis translation.
		\param p_ZAxis the Z axis translation.
	*/
	void Translate(float p_XAxis, float p_YAxis, float p_ZAxis) {
		m_Position += glm::vec3(p_XAxis, p_YAxis, p_ZAxis) * m_Orientation;
	}

	/*!
		\brief Rotates the camera component.
		\param p_Angle the rotation angle.
		\param p_Axis the axis to rotate around.
	*/
	void Rotate(float p_Angle, const glm::vec3 &p_Axis) {
		m_Orientation *= glm::angleAxis(p_Angle, p_Axis * m_Orientation);
	}
	/*!
		\brief Rotates the camera component.
		\param p_Angle the rotation angle.
		\param p_XAxis the X axis value to rotate around.
		\param p_YAxis the Y axis value to rotate around.
		\param p_ZAxis the Z axis value to rotate around.
	*/
	void Rotate(float p_Angle, float p_XAxis, float p_YAxis, float p_ZAxis) {
		m_Orientation *= glm::angleAxis(p_Angle, glm::vec3(p_XAxis, p_YAxis, p_ZAxis) * m_Orientation);
	}

	/*!
		\brief Rotates the camera component around the Y axis.
		\param p_Angle the rotation angle.
	*/
	void Yaw(float p_Angle) { // Y Axis.
		Rotate(p_Angle, 0.0f, 1.0f, 0.0f);
	}
	/*!
		\brief Rotates the camera component around the X axis.
		\param p_Angle the rotation angle.
	*/
	void Pitch(float p_Angle) { // X Axis.
		Rotate(p_Angle, 1.0f, 0.0f, 0.0f);
	}
	/*!
		\brief Rotates the camera component around the Z axis.
		\param p_Angle the rotation angle.
	*/
	void Roll(float p_Angle) { // Z Axis.
		Rotate(p_Angle, 0.0f, 0.0f, 1.0f);
	}

	/*!
		\brief Sets the field of view, of the camera.
		\param p_FieldOFView the new field of view value.
	*/
	void SetFieldOfView(float p_FieldOFView) {
		m_FieldOfView = p_FieldOFView;
	}

	/*!
		\brief Builds a look at view matrix.
		\param p_Target the center.
	*/
	void LookAt(const glm::vec3 &p_Target) { 
		m_Orientation = (glm::toQuat(glm::lookAt(m_Position, p_Target, glm::vec3(0, 1, 0)))); 
	}

	/*!
		\brief Gets the position of the camera component.
		\return returns the position of the camera component.
	*/
	const glm::vec3 &Position() const { 
		return m_Position; 
	}

	/*!
		\brief Gets the Orientation of the camera component.
		\return returns the Orientation of the camera component.
	*/
	const glm::quat &Orientation() const { 
		return m_Orientation; 
	}

	/*!
		\brief Gets the view matrix of the camera component.
		\return returns the view matrix of the camera component.
	*/
	glm::mat4 GetViewMatrix() const { 
		return glm::translate(glm::mat4_cast(m_Orientation), m_Position);
	}
};