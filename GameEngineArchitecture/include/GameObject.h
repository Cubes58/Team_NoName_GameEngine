/**
@file GameObject.h
@brief An abstract class that stores components, and allows components to be added and accessed.
*/
#pragma once

//#include <unordered_map>
#include <typeindex>
//#include <memory>

#include "HashMap.h"
#include "Component.h"

/*! \class GameObject
	\brief An abstract class that stores components, and allows components to be added and accessed.
*/

class GameObject {
protected:
	//std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Components;	//!< Stores pointers to components, using the typeid of the component to identify it.
	const constexpr static unsigned int s_m_NumberOfComponent = 10u;
	CC::HashMap<std::type_index, Component*> m_Components; //!< Stores pointers to components, using the typeid of the component to identify it.

public:
	/*!
		\brief Constructor.
	*/
	GameObject() : m_Components(s_m_NumberOfComponent) {  };
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
	T *GetComponent() {
		CC::HashNode<std::type_index, Component*> *hashNode = m_Components.Get(typeid(T));
		if(hashNode != nullptr) {
			Component *component = hashNode->m_Value;
			return static_cast<T*>(component);
		}

		// Return null if we don't have a component of that type.
		return nullptr;
	}

	/*!
		\brief Templated method, which adds a component to the object.
		\param p_Component the component to add, to the object.
	*/
	template <typename T>
	void AddComponent(T *p_Component) {
		// Add the component to hsh map with its typeid.
		m_Components.Insert(typeid(T), p_Component);
	}
};