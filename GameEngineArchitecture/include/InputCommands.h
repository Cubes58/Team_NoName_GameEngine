/**
@file InputCommands.h
@brief A class that stores the Input Command variations.
*/
#pragma once

#include <vector>
#include <string>
#include <any>
#include <iostream>

#include "Game.h"
#include "DefaultGame.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "PlayerCharacter.h"

class InputCommand;
// Function pointer.
typedef std::shared_ptr<InputCommand>(__stdcall *CreateInputCommand)(std::vector<std::any>); //!< A shared pointer, used in the factory to store creation methods.

/*! \class InputCommand
	\brief An abstract base class, used as an interface for Input Commands.
*/
class InputCommand {
private:
	friend InputHandler;	//!< Allow the input handler to access the (public and) private and protected properties of this class. 

protected:
	bool m_WillInvalidateObjects = false;	//!< Used to indicate whether the Input Command invalides the current state of the game.

public:
	virtual ~InputCommand() = default; //!< Default destructor.


	/*!
		\brief A pure virtual method, which child classes have to implement, so it can be called to execute the command.
	*/
	virtual void Execute() = 0;
};

/*! \class NextSceneCommand
	\brief An InputCommand child class, which switches to the next scene.
*/
class NextSceneCommand : public InputCommand {
private:
	std::weak_ptr<DefaultGame> m_Game;	//!< Stores a weak pointer to the game object.

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	NextSceneCommand(std::vector<std::any> p_Arguments) {
		m_WillInvalidateObjects = true;
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_Game = std::dynamic_pointer_cast<DefaultGame>(std::any_cast<std::shared_ptr<Game>>(p_Arguments[0]));
		}
	}

	/*!
		\brief Called to execute the command.
	*/	
	void Execute() {
		if (auto validGameObject = m_Game.lock())
			validGameObject->SetScene(validGameObject->GetNextScene());
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<NextSceneCommand>(p_ObjectArguments);
	};
};

/*! \class PreviousSceneCommand
	\brief An InputCommand child class, which switches to the previous scene.
*/
class PreviousSceneCommand : public InputCommand {
private:
	std::weak_ptr<DefaultGame> m_Game;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	PreviousSceneCommand(std::vector<std::any> p_Arguments) {
		m_WillInvalidateObjects = true;
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_Game = std::dynamic_pointer_cast<DefaultGame>(std::any_cast<std::shared_ptr<Game>>(p_Arguments[0]));
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_Game.lock())
			validGameObject->SetScene(validGameObject->GetPreviousScene());
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<PreviousSceneCommand>(p_ObjectArguments);
	};
};

/*! \class ReloadInputConfigurationCommand
	\brief An InputCommand child class, which reloads the key configuration file.
*/
class ReloadInputConfigurationCommand : public InputCommand {
private:
	std::weak_ptr<InputHandler> m_InputHandler;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	ReloadInputConfigurationCommand(std::vector<std::any> p_Arguments) {
		m_WillInvalidateObjects = true;
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_InputHandler = std::any_cast<std::shared_ptr<InputHandler>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validInputHandler = m_InputHandler.lock()) {
			validInputHandler->LoadKeyBinds(validInputHandler->GetKeyBindFile());
		}
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<ReloadInputConfigurationCommand>(p_ObjectArguments);
	};
};

/*! \class RotatePlayerLeftCommand
	\brief An InputCommand child class, which rotates a game object left.
*/
class RotatePlayerLeftCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	RotatePlayerLeftCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("RotateCameraLeft");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<RotatePlayerLeftCommand>(p_ObjectArguments);
	};
};

/*! \class RotatePlayerRightCommand
	\brief An InputCommand child class, which rotates a game object right.
*/
class RotatePlayerRightCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	RotatePlayerRightCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if(auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("RotateCameraRight");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<RotatePlayerRightCommand>(p_ObjectArguments);
	};
};

/*! \class StopRotateRightCommand
	\brief An InputCommand child class, which rotates a game object right.
*/
class StopRotateRightCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	StopRotateRightCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("StopRotateCameraRight");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<StopRotateRightCommand>(p_ObjectArguments);
	};
};

/*! \class StopRotateLeftCommand
	\brief An InputCommand child class, which rotates a game object right.
*/
class StopRotateLeftCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	StopRotateLeftCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("StopRotateCameraLeft");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<StopRotateLeftCommand>(p_ObjectArguments);
	};
};

/*! \class MovePlayerForwardCommand
	\brief An InputCommand child class, which moves a game object forward.
*/
class MovePlayerForwardCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	MovePlayerForwardCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("MovePlayerForward");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<MovePlayerForwardCommand>(p_ObjectArguments);
	};
};

/*! \class MovePlayerBackwardCommand
	\brief An InputCommand child class, which moves a game object backward.
*/
class MovePlayerBackwardCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	MovePlayerBackwardCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("MovePlayerBackward");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<MovePlayerBackwardCommand>(p_ObjectArguments);
	};
};

/*! \class MovePlayerLeftCommand
	\brief An InputCommand child class, which moves a game object left.
*/
class MovePlayerLeftCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	MovePlayerLeftCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("MovePlayerLeft");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<MovePlayerLeftCommand>(p_ObjectArguments);
	};
};

/*! \class MovePlayerRightCommand
	\brief An InputCommand child class, which moves a game object right.
*/
class MovePlayerRightCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	MovePlayerRightCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("MovePlayerRight");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<MovePlayerRightCommand>(p_ObjectArguments);
	};
};

/*! \class StopForwardCommand
	\brief An InputCommand child class, which :D.
*/
class StopForwardCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	StopForwardCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("StopPlayerForward");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<StopForwardCommand>(p_ObjectArguments);
	};
};

/*! \classStopBackwardCommand
	\brief An InputCommand child class, which :D.
*/
class StopBackwardCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	StopBackwardCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("StopPlayerBackward");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<StopBackwardCommand>(p_ObjectArguments);
	};
};

/*! \classStopLeftCommand
	\brief An InputCommand child class, which :D.
*/
class StopLeftCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	StopLeftCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("StopPlayerLeft");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<StopLeftCommand>(p_ObjectArguments);
	};
};

/*! \classStopLeftCommand
	\brief An InputCommand child class, which :D.
*/
class StopRightCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	StopRightCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("StopPlayerRight");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<StopRightCommand>(p_ObjectArguments);
	};
};

/*! \class SetCameraFirstPersonPerspectiveCommand
	\brief An InputCommand child class, which sets the camera component's perspective to first person.
*/
class SetCameraFirstPersonPerspectiveCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	SetCameraFirstPersonPerspectiveCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("SetCameraFirstPerson");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<SetCameraFirstPersonPerspectiveCommand>(p_ObjectArguments);
	};
};

/*! \class SetCameraThirdPersonPerspectiveCommand
	\brief An InputCommand child class, which sets the camera component's perspective to third person.
*/
class SetCameraThirdPersonPerspectiveCommand : public InputCommand {
private:
	std::weak_ptr<GameObject> m_GameObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	SetCameraThirdPersonPerspectiveCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value())
				m_GameObject = std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]);
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validGameObject = m_GameObject.lock())
			validGameObject->OnMessage("SetCameraThirdPerson");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<SetCameraThirdPersonPerspectiveCommand>(p_ObjectArguments);
	};
};

/*! \class SetCameraPerspectiveCommand
	\brief An InputCommand child class, which sets the camera component's perspective.
*/
class SetCameraPerspectiveCommand : public InputCommand {
private:
	std::weak_ptr<PlayerCharacter> m_PlayerObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	SetCameraPerspectiveCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value()) 
				m_PlayerObject = std::dynamic_pointer_cast<PlayerCharacter>(std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]));
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validPlayerObject = m_PlayerObject.lock()) {
			CameraViewState playerPerspective = validPlayerObject->CameraPerspective();

			playerPerspective = static_cast<CameraViewState>(static_cast<unsigned int>(playerPerspective) + 1);
			if (playerPerspective == CameraViewState::END)
				playerPerspective = CameraViewState::FIRST_PERSON_CAMERA;

			if (playerPerspective == CameraViewState::FIRST_PERSON_CAMERA)
				validPlayerObject->OnMessage("SetCameraFirstPerson");
			else
				validPlayerObject->OnMessage("SetCameraThirdPerson");
		}
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<SetCameraPerspectiveCommand>(p_ObjectArguments);
	};
};

/*! \class SetCameraInverseRotationCommand
	\brief An InputCommand child class, which sets whether the camera component's rotation is inverted or not.
*/
class SetCameraInverseRotationCommand : public InputCommand {
private:
	std::weak_ptr<PlayerCharacter> m_PlayerObject;

public:
	/*!
		\brief Constructor.
		\param p_Arguments the arguments required.
	*/
	SetCameraInverseRotationCommand(std::vector<std::any> p_Arguments) {
		if (p_Arguments.size() > 0) {
			if (p_Arguments[0].has_value()) 
				m_PlayerObject = std::dynamic_pointer_cast<PlayerCharacter>(std::any_cast<std::shared_ptr<GameObject>>(p_Arguments[0]));
		}
	}

	/*!
		\brief Called to execute the command.
	*/
	void Execute() {
		if (auto validPlayerObject = m_PlayerObject.lock())
			validPlayerObject->InverseRotation() ? validPlayerObject->OnMessage("NoInvertCameraRotation") : validPlayerObject->OnMessage("InvertCameraRotation");
	}

	/*!
		\brief Static function, called to create a generic Inputcommand pointer to this class type.
		\param p_ObjectArguments the arguments required.
		\return Returns a pointer to an InputCommand object, which now refers to this class type.
	*/
	static std::shared_ptr<InputCommand> __stdcall Create(std::vector<std::any> p_ObjectArguments) {
		return std::make_shared<SetCameraInverseRotationCommand>(p_ObjectArguments);
	};
};