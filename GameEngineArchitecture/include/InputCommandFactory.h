/**
@file InputCommandFactory.h
@brief A class that creates Input commands, using the factory pattern.
*/
#pragma once

#include <map>
#include <string>
#include <memory>
#include <utility>

#include "InputCommands.h"
#include "CommandArgumentTypes.h"

/*! \class InputCommandFactory
	\brief A class that creates Input commands, using the factory pattern.
*/

class InputCommandFactory {
private:
	std::map<std::string, std::pair<CommandArgumentType, CreateInputCommand>> m_FactoryMap;	//!< A map that stores function pointers, to create input commands.

	/*!
		\brief Default constructor.
	*/
	InputCommandFactory() {
		Register("NextSceneCommand", CommandArgumentType::GAME, &NextSceneCommand::Create);
		Register("PreviousSceneCommand", CommandArgumentType::GAME, &PreviousSceneCommand::Create);
		Register("ReloadInputConfigurationCommand", CommandArgumentType::INPUT_HANDLER, &ReloadInputConfigurationCommand::Create);

		Register("RotatePlayerLeftCommand", CommandArgumentType::PLAYER, &RotatePlayerLeftCommand::Create);
		Register("RotatePlayerRightCommand", CommandArgumentType::PLAYER, &RotatePlayerRightCommand::Create);
		Register("StopRotateLeftCommand", CommandArgumentType::PLAYER, &StopRotateLeftCommand::Create);
		Register("StopRotateRightCommand", CommandArgumentType::PLAYER, &StopRotateRightCommand::Create);

		Register("MovePlayerForwardCommand", CommandArgumentType::PLAYER, &MovePlayerForwardCommand::Create);
		Register("MovePlayerBackwardCommand", CommandArgumentType::PLAYER, &MovePlayerBackwardCommand::Create);
		Register("MovePlayerLeftCommand", CommandArgumentType::PLAYER, &MovePlayerLeftCommand::Create);
		Register("MovePlayerRightCommand", CommandArgumentType::PLAYER, &MovePlayerRightCommand::Create);

		Register("StopForwardCommand", CommandArgumentType::PLAYER, &StopForwardCommand::Create);
		Register("StopBackwardCommand", CommandArgumentType::PLAYER, &StopBackwardCommand::Create);
		Register("StopLeftCommand", CommandArgumentType::PLAYER, &StopLeftCommand::Create);
		Register("StopRightCommand", CommandArgumentType::PLAYER, &StopRightCommand::Create);

		Register("SetCameraFirstPersonPerspectiveCommand", CommandArgumentType::PLAYER, &SetCameraFirstPersonPerspectiveCommand::Create);
		Register("SetCameraThirdPersonPerspectiveCommand", CommandArgumentType::PLAYER, &SetCameraThirdPersonPerspectiveCommand::Create);
		Register("SetCameraPerspectiveCommand", CommandArgumentType::PLAYER, &SetCameraPerspectiveCommand::Create);
		Register("SetCameraInverseRotationCommand", CommandArgumentType::PLAYER, &SetCameraInverseRotationCommand::Create);
	}
	/*!
		\brief Constructor.
		\param InputCommandFactory
	*/
	InputCommandFactory(const InputCommandFactory &) = default;
	InputCommandFactory &operator=(const InputCommandFactory &) { 
		return *this; 
	}	//!< Set the assignment operator.

public:
	/*!
		\brief Default destructor - clears the mapped function pointers, used to create different types of input commands.
	*/
	~InputCommandFactory() { 
		m_FactoryMap.clear(); 
	}

	/*!
		\brief Get the InputCommandFactory instance.
		\return Returns an InputCommandFactory instance, to be used.
	*/
	static InputCommandFactory &Instance() {
		static InputCommandFactory s_InputCommandFactory;

		return s_InputCommandFactory;
	}

	/*!
		\brief Registers an Input Command type.
		\param p_InputCommandName is the name of the Input Command (child) class, that will be used to identify that class.
		\param p_CommandDataType justifies the data the class uses.
		\param p_CreateInputCommandPointer is the function pointer, which is used to create that type of Input Command class.
	*/
	void Register(const std::string &p_InputCommandName, CommandArgumentType p_CommandDataType, CreateInputCommand p_CreateInputCommandPointer) {
		m_FactoryMap[p_InputCommandName] = std::make_pair(p_CommandDataType, p_CreateInputCommandPointer);
	}

	/*!
		\brief Gets a pointer to a (child-class) type of Input Command, and returns it as an InputHandler pointer, to be used/stored.
		\param p_InputCommandName is the name of the Input Command (child) class, that will be used to identify that class.
		\param p_Data passes the data that the class requires.
	*/
	std::shared_ptr<InputCommand> CreateInputCommandType(const std::string &p_InputCommandName, std::vector<std::any> p_Data) {
		std::map<std::string, std::pair<CommandArgumentType, CreateInputCommand>>::iterator iter = m_FactoryMap.find(p_InputCommandName);

		if (iter != m_FactoryMap.end())
			return iter->second.second(p_Data);

		return nullptr;
	}

	/*!
		\brief Gets a pointer to a (child-class) type of Input Command, and returns it as an InputHandler pointer, to be used/stored.
		\param p_InputCommandName is the name of the Input Command (child) class, that will be used to identify that class.
	*/
	std::shared_ptr<InputCommand> CreateInputCommandType(const std::string &p_InputCommandName) {
		std::vector<std::any> temp;
		return CreateInputCommandType(p_InputCommandName, temp);
	}

	/*!
		\brief Gets the command argument type, given the input command name.
		\param p_InputCommandName is the name of the Input Command (child) class, that's used to identify that class.
		\return Returns the CommandArgumentType, so what data the InputCommand requires, to function, will return CommandArgumentType::NONE if no data is required.
	*/
	const CommandArgumentType GetCommandType(const std::string &p_InputCommandName) {
		std::map<std::string, std::pair<CommandArgumentType, CreateInputCommand>>::iterator iter = m_FactoryMap.find(p_InputCommandName);

		if (iter != m_FactoryMap.end())
			return iter->second.first;

		return CommandArgumentType::NONE;
	}
};