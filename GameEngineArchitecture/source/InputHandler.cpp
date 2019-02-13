#include "InputHandler.h"

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <stdexcept>

#include "InputCommands.h"
#include "InputCommandFactory.h"
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include "PlayerCharacter.h"

InputHandler::InputHandler(std::weak_ptr<Game> p_Game) {
	m_Game = p_Game;
}

bool InputHandler::LoadKeyBinds(const std::string &p_KeyConfigurationFile) {
	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	m_ReleaseControlMapping.clear();
	m_RepeatControlMapping.clear();

	try {
		jsonData.open(p_KeyConfigurationFile.c_str());
		// Check for errors, when parsing the file data.
		if (!reader.parse(jsonData, root)) {
			std::cerr << "Failed to parse data from: " << p_KeyConfigurationFile << reader.getFormattedErrorMessages()<< "\n";
			throw std::invalid_argument("Couldn't parse file.");
		}

		if (jsonData.is_open())
			jsonData.close();
	}
	catch (const std::invalid_argument &p_Exception) {
		std::cerr << "INVALID_ARGUMENT::INPUT_HANDLER: " << p_Exception.what() << "\n";
		if (jsonData.is_open())
			jsonData.close();

		++m_NumberOfAttempts;	// Attempt to load the default key configuration file, if the one it's tried is broken.
		if (m_NumberOfAttempts < s_MAX_ATTEMPTS_TO_LOAD_KEY_CONFIGURATION_FILE)
			LoadKeyBinds(s_DEFAULT_CONFIGURATION_FILE);
		else
			return false;
	}
	catch (...) {
		// Catch all rune-time errors, and attempt to recover.
		std::cerr << "Something unexpected happened, while reading the key configuration file: " << p_KeyConfigurationFile 
			<< "\nAttempting to load the key configuration file again!" << std::endl;
		if(jsonData.is_open())
			jsonData.close();

		++m_NumberOfAttempts;
		if (m_NumberOfAttempts < s_MAX_ATTEMPTS_TO_LOAD_KEY_CONFIGURATION_FILE - 1) {
			LoadKeyBinds(p_KeyConfigurationFile);
		}
		else if (m_NumberOfAttempts == s_MAX_ATTEMPTS_TO_LOAD_KEY_CONFIGURATION_FILE) {
			LoadKeyBinds(s_DEFAULT_CONFIGURATION_FILE);	
		}
		else {
			return false;
		}
	}

	const Json::Value keyBinds = root["KeyBindings"];

	std::vector<KeyInformation> keyBindings;
	for (int i = 0; i < keyBinds.size(); i++) {
		// Load the key.
		const Json::Value keyNode = keyBinds[i]["key"];
		std::string key = "error";
		if (keyNode.type() != Json::nullValue) {
			key = keyNode.asString();
		}

		// Load the command.
		const Json::Value commandNameNode = keyBinds[i]["command"];
		std::string commandName = "error";
		if (commandNameNode.type() != Json::nullValue) {
			commandName = commandNameNode.asString();
		}

		// Load the input type (when should the action take place, on release or on press/repeat).
		const Json::Value typeNode = keyBinds[i]["inputType"];
		std::string typeName = "press";		// Default value is on press.
		if (typeNode.type() != Json::nullValue) {
			typeName = typeNode.asString();
		}

		keyBindings.emplace_back(key, commandName, typeName);
	}

	return ApplyKeyBinds(keyBindings);
}

bool InputHandler::ApplyKeyBinds(std::vector<KeyInformation> &p_KeyBinds) {
	bool successfullyBoundAllKeysRead = true;
	for (auto keyBind : p_KeyBinds) {
		// Get the key int value:
		int key = -1;
		if (keyBind.m_Key.size() > 0)
			key = (unsigned int)keyBind.m_Key[0];

		// Get the command:
		std::shared_ptr<InputCommand> command = LoadCommandType(keyBind);

		// Get the input type, and ensure the ket is a valid value:
		if (keyBind.m_InputType == "press" && key > 0) {
			m_RepeatControlMapping.emplace(key, command);
		}
		else if (keyBind.m_InputType == "release" && key > 0) {
			m_ReleaseControlMapping.emplace(key, command);
		}
		else {
			std::cerr << "ERROR::LOADING_KEY_BINDS:\n" 
				<< "Key: " << keyBind.m_Key << "\n"
				<< "Command: " << keyBind.m_Command << "\n"
				<< "InputType: " << keyBind.m_InputType << "\n" << std::endl;
			successfullyBoundAllKeysRead = false;
		}
	}

	return successfullyBoundAllKeysRead;
}

std::shared_ptr<InputCommand> InputHandler::LoadCommandType(const KeyInformation &p_KeyInformation) {
	CommandArgumentType commandArgumentType = InputCommandFactory::Instance().GetCommandType(p_KeyInformation.m_Command);

	// Passes the required data, to the command object.
	std::vector<std::any> data;
	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>>::iterator playerIter;
	switch (commandArgumentType) {
	case CommandArgumentType::GAME_OBJECTS:
		for (auto object : m_ObjectsRequiringInput)
			data.push_back(object.second);
		break;
	case CommandArgumentType::PLAYER:
		playerIter = m_ObjectsRequiringInput.find(typeid(PlayerCharacter));
		if (playerIter != m_ObjectsRequiringInput.end())
			data.push_back(playerIter->second);
		break;
	case CommandArgumentType::INPUT_HANDLER:
		data.push_back(this->shared_from_this());
		break;
	case CommandArgumentType::GAME:
		if (auto validGameObject = m_Game.lock())
			data.push_back(std::any(validGameObject));
		break;
	case CommandArgumentType::NONE:
		return InputCommandFactory::Instance().CreateInputCommandType(p_KeyInformation.m_Command);
		break;

	default:
		std::cerr << "CommandArgumentType invalid, value: " << static_cast<unsigned int>(commandArgumentType) << std::endl;
		return std::shared_ptr<InputCommand>(nullptr);
		break;
	}

	return InputCommandFactory::Instance().CreateInputCommandType(p_KeyInformation.m_Command, data);
}

void InputHandler::HandleInputs(const std::vector<bool> &p_KeyPressBuffer, std::vector<bool> &p_KeyReleaseBuffer) {
	for (const auto &mapEntry : m_RepeatControlMapping) {
		if (p_KeyPressBuffer[mapEntry.first]) {
			bool willInvalidateObject = mapEntry.second->m_WillInvalidateObjects;

			mapEntry.second->Execute();
			if (willInvalidateObject)
				break;
		}
	}

	for (const auto &mapEntry : m_ReleaseControlMapping) {
		if (p_KeyReleaseBuffer[mapEntry.first]) {
			// Must manually reset the key, as there's no other indication after the key release event.
			p_KeyReleaseBuffer[mapEntry.first] = false;
			bool willInvalidateObjects = mapEntry.second->m_WillInvalidateObjects;

			mapEntry.second->Execute();
			if (willInvalidateObjects)
				break;
		}
	}
}

void InputHandler::HandleCursorInput(double p_MouseXPosition, double p_MouseYPosition) {
	static double s_PreviousMouseXPosition = p_MouseXPosition;
	static double s_PreviousMouseYPosition = p_MouseYPosition;

	auto iter = m_ObjectsRequiringInput.find(typeid(PlayerCharacter));
	if (iter != m_ObjectsRequiringInput.end())
		std::dynamic_pointer_cast<PlayerCharacter>(iter->second)->HandleCursorInput(p_MouseXPosition - s_PreviousMouseXPosition, p_MouseYPosition - s_PreviousMouseYPosition);

	s_PreviousMouseXPosition = p_MouseXPosition;
	s_PreviousMouseYPosition = p_MouseYPosition;
}

void InputHandler::SetObjectsRequiringInput(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> p_ObjectsRequiringInput) {
	m_ObjectsRequiringInput = p_ObjectsRequiringInput;
}