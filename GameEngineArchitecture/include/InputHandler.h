/**
@file InputHandler.h
@brief A class that deals with the handling of input.
*/
#pragma once

#include <map>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>

class Game;
class InputCommand;
class GameObject;
class PlayerCharacter;

static constexpr unsigned int s_MAX_ATTEMPTS_TO_LOAD_KEY_CONFIGURATION_FILE = 5;
static const std::string s_DEFAULT_CONFIGURATION_FILE = "resources/keyBindings/default.json";

/*! \class InputHandler
	\brief A class that deals with the handling of input.
*/
class InputHandler : public std::enable_shared_from_this<InputHandler> {
private:
	/**
		* A structure to represent Key inputs Information.
	*/
	struct KeyInformation {
		KeyInformation() = default;	//!< Default constructor.

		/*! 
			\brief Constructor.
			\param p_Key the key code.
			\param p_Command the command triggered, when the key input is processed.
			\param p_InputType when the event occurs - key press or release.
		*/
		KeyInformation(const std::string &p_Key, const std::string &p_Command, const std::string &p_InputType)
			: m_Key(p_Key), m_Command(p_Command), m_InputType(p_InputType) {
		}
		std::string m_Key;	//!< The key code.
		std::string m_Command;	//!< The command.
		std::string m_InputType; //!< The input type (on release or press).
	};

	// Input bindings.
	std::map<unsigned int, std::shared_ptr<InputCommand>> m_RepeatControlMapping;	//!< Stores input commands for press/on repeat key event commands.
	std::map<unsigned int, std::shared_ptr<InputCommand>> m_ReleaseControlMapping;	//!< Stores input commands for on release key event commands.
	std::string m_KeyBindFile = "resources/keyBindings/userConfiguration.json";	//!< The key bind file used, to load in the key configuration settings.

	// Game objects that require user input.
	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> m_ObjectsRequiringInput;	//!< Stores the objects that require input.
	std::weak_ptr<Game> m_Game;	//!< A weak pointer to the game object, used to pass into certian commands.

	unsigned int m_NumberOfAttempts = 0;	//!< The number of attempts made at loading the key file configuration.

	/*!
		\brief Applies the key binds, and adds them to the respective ControlMapping map.
		\param p_KeyBinds a vector of key Information.
		\return Returns true if all of the key binds were able to be applied, otherwise false.
	*/
	bool ApplyKeyBinds(std::vector<KeyInformation> &p_KeyBinds);
	/*!
		\brief Gets the data required for the input command, and creates it.
		\param p_KeyInformation the key information.
		\return Returns an InputCommand to the specified Input Command type.
	*/
	std::shared_ptr<InputCommand> LoadCommandType(const KeyInformation &p_KeyInformation);

public:
	/*!
		\brief Constructor.
		\param p_Game a weak pointer to the game instance.
	*/
	InputHandler(std::weak_ptr<Game> p_Game);
	/*!
		\brief Attempts to load the key configuration file.
		\param p_KeyConfigurationFile the key configuration file to load.
	*/
	bool LoadKeyBinds(const std::string &p_KeyConfigurationFile);

	/*!
		\brief Handles keyboard input events (release and press/repeat).
		\param p_KeyPressBuffer key press events, vector.
		\param p_KeyReleaseBuffer key release events, vector.
	*/
	void HandleInputs(const std::vector<bool> &p_KeyPressBuffer, std::vector<bool> &p_KeyReleaseBuffer);
	/*!
		\brief Handles cursor input event.
		\param p_MouseXPosition the X axis position of the mouse.
		\param p_MouseYPosition the Y axis position of the mouse.
	*/
	void HandleCursorInput(double p_MouseXPosition, double p_MouseYPosition);

	/*!
		\brief Sets the game objects that require input.
		\param p_ObjectsRequiringInput the objects requiring input.
	*/
	void SetObjectsRequiringInput(std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> p_ObjectsRequiringInput);

	/*!
		\brief Get the key configuration file.
		\return Returns the key configuration file.
	*/
	inline const std::string &GetKeyBindFile() const {
		return m_KeyBindFile;
	}
};