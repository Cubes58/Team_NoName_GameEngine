#include "GLFW_EngineCore.h"

#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <limits>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "RenderEngine.h"

#include "Game.h"
#include "InputHandler.h"
#include "ModelComponent.h"
#include "CameraComponent.h"

std::vector<bool> GLFW_EngineCore::m_KeyPressBuffer;
std::vector<bool> GLFW_EngineCore::m_KeyReleaseBuffer;
int GLFW_EngineCore::m_ScreenWidth;
int GLFW_EngineCore::m_ScreenHeight;
double GLFW_EngineCore::m_MouseXPosition;
double GLFW_EngineCore::m_MouseYPosition;
bool GLFW_EngineCore::m_CaptureMouse = true;

static constexpr unsigned int s_STARTING_LEVEL = 1;

GLFW_EngineCore::~GLFW_EngineCore() {
	// Clean up.
	glfwTerminate();
}

bool GLFW_EngineCore::InitWindow(int p_Width, int p_Height, const std::string &p_WindowName) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_ScreenWidth = p_Width;
	m_ScreenHeight = p_Height;

	m_Window = glfwCreateWindow(p_Width, p_Height, p_WindowName.c_str(), nullptr, nullptr);
	if (m_Window == nullptr) {
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// Callback functions.
	glfwSetFramebufferSizeCallback(m_Window, WindowResizeCallbackEvent);
	glfwSetKeyCallback(m_Window, KeyCallbackEvent);

	// Mouse movement.
	glfwSetCursorPosCallback(m_Window, MouseMoveCallbackEvent);
	CaptureMouse(m_Window);

	// Make space for the keybuffer.
	m_KeyPressBuffer.resize(m_KeyBufferSize);
	std::fill(m_KeyPressBuffer.begin(), m_KeyPressBuffer.end(), false);

	m_KeyReleaseBuffer.resize(m_KeyBufferSize);
	std::fill(m_KeyReleaseBuffer.begin(), m_KeyReleaseBuffer.end(), false);

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	// Enable alpha transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable face culling.
	glEnable(GL_CULL_FACE);

	glfwSwapInterval(0);

	return true;
}

bool GLFW_EngineCore::RunEngine(std::shared_ptr<Game> p_Game) {
  m_PhysicsEngine = new PhysicsEngine(p_Game);
	RenderEngineInstance.Init(m_ScreenWidth, m_ScreenHeight);
  
	p_Game->m_EngineInterface = std::make_shared<GLFW_EngineCore>(*this);
	p_Game->m_InputHandler = std::make_shared<InputHandler>(p_Game);
	p_Game->SetScene(s_STARTING_LEVEL);

	// Game loop.
	while (!glfwWindowShouldClose(m_Window) && p_Game->IsRunning()) {

		p_Game->m_InputHandler->HandleInputs(m_KeyPressBuffer, m_KeyReleaseBuffer);
		p_Game->m_InputHandler->HandleCursorInput(m_MouseXPosition, m_MouseYPosition);

		m_PhysicsEngine->Update();
		p_Game->Update((float)deltaTime); // Update game logic.
		p_Game->Render(); // Prepare game to send information to the renderer in engine core.
		

		// Swap the buffers.
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	return true;
}

void GLFW_EngineCore::RenderColouredBackground(float p_Red, float p_Green, float p_Blue) {
	glClearColor(p_Red, p_Green, p_Blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFW_EngineCore::MouseMoveCallbackEvent(GLFWwindow *p_Window, double p_XPosition, double p_YPosition) {
	m_MouseXPosition = p_XPosition;
	m_MouseYPosition = p_YPosition;
}

void GLFW_EngineCore::CaptureMouse(GLFWwindow *p_Window) {
	if (m_CaptureMouse) 
		glfwSetInputMode(p_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Capture the mouse cursor.
	else 
		glfwSetInputMode(p_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	// Release the mouse cursor.
}

//-----------------------------Private functions------------------------------
void GLFW_EngineCore::KeyCallbackEvent(GLFWwindow *p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Mods) {
	if (p_Key == GLFW_KEY_UNKNOWN || p_Key > m_KeyBufferSize) {
		return;
	}
	m_KeyPressBuffer[p_Key] = ((p_Action == GLFW_PRESS || p_Action == GLFW_REPEAT));
	m_KeyReleaseBuffer[p_Key] = (p_Action == GLFW_RELEASE);

	if ((p_Key == GLFW_KEY_DELETE) && (p_Action == GLFW_RELEASE)) {
		m_CaptureMouse = !m_CaptureMouse;
		CaptureMouse(p_Window);
	}
	if (glfwGetKey(p_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(p_Window, true);
	}
}

void GLFW_EngineCore::WindowResizeCallbackEvent(GLFWwindow *p_Window, int p_Width, int p_Height) {
	// Change the opengl viewport to match the new m_window size.
	m_ScreenWidth = p_Width;
	m_ScreenHeight = p_Height;
	glViewport(0, 0, p_Width, p_Height);
}
