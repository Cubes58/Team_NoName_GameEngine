/**
@file Game.h
@brief An abstract base class that manages the scenes, and input handler.
*/
#pragma once

#include <memory>

class IEngineCore;
class Scene;
class InputHandler;

/*! \class Game
	\brief An abstract base class that manages the scenes, and input handler.
*/
class Game {
protected:
	std::shared_ptr<Scene> m_CurrentScene;	//!< Stores the current scene.
	bool m_IsRunning = true;	//!< Stores whether the game is/should be running.

public:
	Game() = default;	//!< Default constructor.
	virtual ~Game() = default;	//!< Default destructor.

	std::shared_ptr<IEngineCore> m_EngineInterface;	//!< A pointer to the engine.
	std::shared_ptr<InputHandler> m_InputHandler;	//!< A pointer to the input handler.

	/*!
		\brief A pure virtual method, used to set the scene.
		\param p_SceneNumber the scene number to switch to.
	*/
	virtual void SetScene(unsigned int p_SceneNumber) = 0;
	/*!
		\brief A pure virtual method, used to update the game.
		\param p_DeltaTime the amount of time passed since the last update.
	*/
	
	virtual void imguiInit() = 0;
	virtual void imguiRender() = 0;
	virtual void imguiShutdown() = 0;

	virtual void Update(float p_DeltaTime) = 0;
	/*!
		\brief A pure virtual method, used to render the game.
	*/
	virtual void Render() = 0;

	void CloseGame() {
		m_IsRunning = false;
	}

	/*!
		\brief Gets whether the game is running/on-going.
		\return Returns true if the game is running, false otherwise.
	*/
	inline const bool &IsRunning() {
		return m_IsRunning;
	}
};