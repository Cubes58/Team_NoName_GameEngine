/**
@file GLFW_EngineCore.h
@brief GLFW engine class.
*/
#pragma once

#include <vector>
#include <map>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include "ShaderProgram.h"
#include "FontRenderer.h"

#include "IEngineCore.h"

class Game;
class Model;
class CameraComponent;

/*! \class GLFW_EngineCore
	\brief GLFW engine class.
*/

class GLFW_EngineCore : public IEngineCore {
private:
	GLFWwindow* m_Window;	//!< The window, where everything is rendered.

	static int m_ScreenWidth;	//!< The screen width.
	static int m_ScreenHeight;	//!< The screen height.
	static std::vector<bool> m_KeyPressBuffer;	//!< A buffer of key press states.
	static std::vector<bool> m_KeyReleaseBuffer;	//!< A buffer of key release states.
	static const int m_KeyBufferSize = 400;	//!< Size of the key buffer states.

	// Mouse values.
	static double m_MouseXPosition;	//!< The X axis mouse position.
	static double m_MouseYPosition;	//!< The Y axis mouse position.
	static bool m_CaptureMouse;	//!< Stores whether the user wants the mouse to be locked within the screen, or not.

	/*!
		\brief Called when a mouse movement event is generated.
		\param p_Window the window to check for mouse movement with.
		\param p_XPosition the X position of the mouse.
		\param p_YPosition the Y position of the mouse.
	*/
	static void MouseMoveCallbackEvent(GLFWwindow *p_Window, double p_XPosition, double p_YPosition);
	/*!
		\brief Captures/releases the mouse.
		\param p_Window the window to check for mouse movement with.
	*/
	static void CaptureMouse(GLFWwindow *p_Window);
	/*!
		\brief Called a key event happens.
		\param p_Window the window to check for mouse movement with.
		\param p_Key the key code of the key event.
		\param p_ScanCode the scan code.
		\param p_Action the action of the key event (release or press/repeat).
		\param p_Mods the mods on the keyboard.
	*/
	static void KeyCallbackEvent(GLFWwindow *p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Mods);
	/*!
		\brief Resize the window.
		\param p_Window the window to check for mouse movement with.
		\param p_Width the width to resize the window.
		\param p_Height the height to resize the window.
	*/
	static void WindowResizeCallbackEvent(GLFWwindow *p_Window, int p_Width, int p_Height);

	ShaderProgram m_DefaultShaderProgram;	//!< The default model rendering shader.
	
	
	/*!
		\brief Sets the default model shaders.
	*/
	void SetDefaultShaders();
public:
	GLFW_EngineCore() = default;	//!< Default constructor.
	~GLFW_EngineCore() override;	//!< Default destructor.

	/*!
		\brief Used to initialise the window.
		\param p_Width the window width.
		\param p_Height the window height.
		\param p_WindowName the window name.
	*/
	bool InitWindow(int p_Width, int p_Height, const std::string &p_WindowName) override;
	/*!
		\brief Used run the game (the game loop).
		\param p_Game the game pointer
	*/
	bool RunEngine(std::shared_ptr<Game> p_Game) override;

	/*!
		\brief Used to render a custom background colour, and clear the colour and depth buffers.
		\param p_Red the red colour's strength.
		\param p_Green the green colour's strength.
		\param p_Blue the blue colour's strength.
	*/
	void RenderColouredBackground(float p_Red, float p_Green, float p_Blue) override;

	/*!
		\brief Sets the camera.
		\param p_Camera the camera that will be followed.
	*/
	void SetCamera(const std::shared_ptr<CameraComponent> p_Camera) override;
	/*!
		\brief Used to render a model.
		\param p_Model the pointer to the model that will be rendered.
		\param p_ModelMatrix the model matrix, the position of the model.
	*/
	void DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 &p_ModelMatrix) override;
};