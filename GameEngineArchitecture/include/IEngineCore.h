/**
@file IEngineCore.h
@brief An abstract base class, for the general engine class.
*/
#pragma once

#include <string>
#include <memory>

#include <glm/mat4x4.hpp>

class Game;
class Model;
class CameraComponent;

/*! \class IEngineCore
	\brief An abstract base class, for the general engine class.
*/

class IEngineCore {
public:
	virtual ~IEngineCore() = default;	//!< Default virtual constructor.

	/*!
		\brief A pure virtual method, used to initialise the window.
		\param p_Width the window width.
		\param p_Height the window height.
		\param p_WindowName the window name.
	*/
	virtual bool InitWindow(int p_Width, int p_Height, const std::string &p_WindowName) = 0;
	/*!
		\brief A pure virtual method, used rune the game (the game loop).
		\param p_Game the game pointer
	*/
	virtual bool RunEngine(std::shared_ptr<Game> p_Game) = 0;

	/*!
		\brief A pure virtual method, used to render a custom background colour, and clear the colour and depth buffers.
		\param p_Red the red colour's strength.
		\param p_Green the green colour's strength.
		\param p_Blue the blue colour's strength.
	*/
	virtual void RenderColouredBackground(float p_Red, float p_Green, float p_Blue) = 0;
	
};