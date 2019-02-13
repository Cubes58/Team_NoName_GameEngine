/**
@file GameObject.h
@brief An abstract class that stores components, and allows components to be added and accessed.
*/
#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Component.h"

/*! \class GameObject
	\brief An abstract class that stores components, and allows components to be added and accessed.
*/

class GameObject {
protected:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Components;	//!< Stores pointers to components, using the typeid of the component to identify it.

public:
	GameObject() = default;	//!< Default constructor.
	~GameObject() = default; //!< Default destructor.

	/*!
		\brief Pure virtual method, which can be called on GameObject types, to update them.
		\param p_DeltaTime time passed.
	*/
	virtual void OnUpdate(float p_DeltaTime) = 0;
	/*!
		\brief Pure virtual method, which can be called on GameObject types, to send messages to the object.
		\param p_Message message being sent to the object.
	*/
	virtual void OnMessage(const std::string &p_Message) = 0;

	/*!
		\brief Templated method, which returns the template T component.
		\return Returns the requests component (type), if it exists, else it returns a null pointer. 
	*/
	template <typename T>
	std::shared_ptr<T> GetComponent() {
		auto iter = m_Components.find(typeid(T));

		if (iter != std::end(m_Components)) {
			// If found, dynamic cast the component pointer and return it.
			return std::dynamic_pointer_cast<T>(iter->second);
		}

		// Return null if we don't have a component of that type.
		return nullptr;
	}

	/*!
		\brief Templated method, which adds a component to the object.
		\param p_Component the component to add, to the object.
	*/
	template <typename T>
	void AddComponent(std::shared_ptr<T> p_Component) {
		// Add the component to unoreder map with hash of its typeid.
		m_Components[typeid(T)] = p_Component;
	}
};