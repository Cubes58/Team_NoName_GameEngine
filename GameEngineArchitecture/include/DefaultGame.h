/**
@file DefaultGame.h
@brief A class that manages the scene[s], input handler, and gameplay.
*/
#pragma once

#include <map>
#include <string>
#include <memory>

#include "Game.h"

/*! \class DefaultGame
	\brief A class that manages the scene[s], input handler, and gameplay.
*/
class DefaultGame : public Game, public std::enable_shared_from_this<DefaultGame> {
private:
	unsigned int m_CurrentSceneNumber;	//!< Stores the current scene number.
	std::map<unsigned int, std::string> m_Scenes;	//!< Stores every scene, in the game.
	bool m_SwitchingScene = false;	//!< Stores whether the scene is being switched, or not.

	/*!
		\brief Loads the scenes, from a folder.
		\param p_FolderLocation the scene folder.
	*/
	void LoadScenes(const std::string &p_FolderLocation);
public:
	DefaultGame();	//!< Default constructor.
	~DefaultGame();	//!< Default destructor.

	/*!
		\brief Used to set the scene.
		\param p_SceneNumber the scene number to switch to.
	*/
	void SetScene(unsigned int p_SceneNumber) override;
	/*!
		\brief Used to update the game.
		\param p_DeltaTime the amount of time passed since the last update.
	*/
	void Update(float p_DeltaTime) override;
	/*!
		\brief Used to render the game.
	*/
	void Render() override;

	void imguiInit() override;
	void imguiRender() override;
	void imguiShutdown() override;

	/*!
		\brief Gets the next scene, in m_Scenes map.
		\return Returns the next scene key, identifier.
	*/
	unsigned int GetNextScene() const;
	/*!
		\brief Gets the previous scene, in m_Scenes map.
		\return Returns the previous scene key, identifier.
	*/
	unsigned int GetPreviousScene() const;

	/*!
		\brief Gets the current scene, in the m_Scenes map.
		\return Returns the current scene key, identifier.
	*/
	inline const unsigned int &GetCurrentSceneNumber() const {
		return m_CurrentSceneNumber;
	}
};