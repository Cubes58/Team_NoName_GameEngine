/**
@file ModelComponent.h
@brief A class that stores the properties necessary for a model.
*/
#pragma once

#include <memory>

#include "Component.h"
#include "ModelManager.h"

/*! \class ModelComponent
	\brief A class that stores the properties necessary for a model.
*/

class ModelComponent : public Component {
private:
	std::shared_ptr<Model> m_Model;	//!< A model pointer, pointing to the model that this component uses.

public:
	/*!
		\brief Constructor.
		\param p_ModelName the name of the model.
	*/
	ModelComponent(const std::string &p_ModelName) {
		m_Model = MODEL_MANAGER.GetModel(p_ModelName);
	}
	~ModelComponent() = default;	//!< Default destructor.

	/*!
		\brief Update the attack component.
		\param p_DeltaTime the time passed.
	*/
	void OnUpdate(float p_DeltaTime) override {

	}

	/*!
		\brief Sends a message to the component.
		\param p_Message the message being sent to the component.
	*/
	void OnMessage(const std::string &p_Message) override {
		
	}

	/*!
		\brief Returns the model pointer.
	*/
	std::shared_ptr<Model> GetModel() const {
		return m_Model;
	}
};