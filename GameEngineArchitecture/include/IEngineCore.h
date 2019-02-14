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
	/*!
		\brief A pure virtual method, set the camera.
		\param p_Camera the camera that will be followed.
	*/
	virtual	void SetCamera(const std::shared_ptr<CameraComponent> p_Camera) = 0;
	/*!
		\brief A pure virtual method, used to render a model.
		\param p_Model the pointer to the model that will be rendered.
		\param p_ModelMatrix the model matrix, the position of the model.
	*/
	virtual void DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 &p_ModelMatrix) = 0;
	/*!
		\brief A pure virtual method, used to render text.
		\param p_Text the text to be displayed.
		\param p_XPosition the screen's X position, of where the text should be rendered.
		\param p_YPosition the screen's Y position, of where the text should be rendered.
		\param p_Scale the text's scale.
		\param p_Colour the colour of the text.
	*/
	virtual void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour) = 0;
};