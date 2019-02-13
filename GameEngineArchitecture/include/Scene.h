/**
@file Scene.h
@brief A class that manages the level and gameplay.
*/
#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <memory>

#include <glm/vec3.hpp>

#include "Collision.h"

class IEngineCore;
class InputHandler;
class GameObject;
class PlayerCharacter;
class DefaultGame;

/*! \class Scene
	\brief A class that manages the level and gameplay.
*/
class Scene {
private:
	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> m_GameObjects;	//!< Stores every game object.
	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> m_ObjectsRequiringInput;	//!< Only stores game objects that require input.

	glm::vec3 m_BackgroundColour;	//!< Stores the background colour, for the window.
	Collision m_Collision;	//!< Used for collision testing.

	/*!
		\brief Loads level data from a JSON file.
		\param p_SceneFile the (scene's) JSON file location.
		\param p_Game the game pointer.
		\return Returns true if it loaded everything successfully, otherwise false.
	*/
	bool LoadLevelJSON(const std::string &p_SceneFile, std::shared_ptr<DefaultGame> p_Game);
	/*!
		\brief Unloads the level, by clearing the game object containers.
	*/
	void UnloadLevel();

	/*!
		\brief Displays unsuccessfully loaded models in the window, as (GUI) text.
		\param p_EngineInterface the engine interface pointer.
	*/
	void DisplayUnsuccessfullyLoadedModels(std::shared_ptr<IEngineCore> p_EngineInterface);

public:
	/*!
		\brief Constructor.
		\param p_SceneFile the (scene's) JSON file location.
		\param p_Game a pointer to the game object (must be turned into a weak pointer, if stored).
		\param p_BackgroundColour the colour of the window's background - default initialised.
	*/
	Scene(const std::string &p_SceneFile, std::shared_ptr<DefaultGame> p_Game, glm::vec3 p_BackgroundColour = glm::vec3(0.2f, 0.2f, 0.2f));
	~Scene() = default;	//!< Default destructor.

	/*!
		\brief Updates the scene.
		\param p_DeltaTime time passed.
	*/
	void Update(float p_DeltaTime);
	/*!
		\brief Renders the scene.
		\param p_EngineInterface the engine interface pointer.
	*/
	void Render(std::shared_ptr<IEngineCore> p_EngineInterface);

	/*!
		\brief Gets the objects that require input.
		\return Returns the objects that require objects.
	*/
	std::unordered_multimap<std::type_index, std::shared_ptr<GameObject>> GetObjectsRequiringInput() const;
};