/**
@file Component.h
@brief An abstract base class that has the basic properties a component would need.
*/
#pragma once

#include <string>

/*! \class Component
	\brief An abstract base class that has the basic properties a component would need.
*/

class Component {
public:
	Component() = default;	//!< Default constructor.
	virtual ~Component() = default;	//!< Default destructor.

	/*!
		\brief Pure virtual method, which must be implemented by children classes, that updates the component.
		\param p_DeltaTime the time passed 
	*/
	virtual void OnUpdate(float p_DeltaTime) = 0;

	/*!
		\brief Pure virtual method, which must be implemented by children classes, that sends a message to the component.
		\param p_Message the message being sent to the component.
	*/
	virtual void OnMessage(const std::string &p_Message) = 0;
};