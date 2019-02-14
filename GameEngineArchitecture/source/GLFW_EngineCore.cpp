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

	// Set the shaders to the given default ones.
	SetDefaultShaders();
	SetupDefaultFont();

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	// Enable alpha transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable face culling.
	glEnable(GL_CULL_FACE);

	return true;
}

bool GLFW_EngineCore::RunEngine(std::shared_ptr<Game> p_Game) {
	p_Game->m_EngineInterface = std::make_shared<GLFW_EngineCore>(*this);
	p_Game->m_InputHandler = std::make_shared<InputHandler>(p_Game);
	p_Game->SetScene(s_STARTING_LEVEL);

	auto previousTime = glfwGetTime();
	// Game loop.
	while (!glfwWindowShouldClose(m_Window) && p_Game->IsRunning()) {
		auto currentTime = glfwGetTime();
		auto deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		p_Game->m_InputHandler->HandleInputs(m_KeyPressBuffer, m_KeyReleaseBuffer);
		p_Game->m_InputHandler->HandleCursorInput(m_MouseXPosition, m_MouseYPosition);

		m_DefaultShaderProgram.ErrorChecker();
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

// Loading some default shaders to get things up and running.
void GLFW_EngineCore::SetDefaultShaders() {
	//LoadShader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag", m_DefaultShaderProgram);
	//LoadShader("resources/shaders/fontShader.vert", "resources/shaders/fontShader.frag", m_FontShaderProgram);

	m_DefaultShaderProgram.CompileShader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag");
	m_FontShaderProgram.CompileShader("resources/shaders/fontShader.vert", "resources/shaders/fontShader.frag");
	
	// set the default shader
	glUseProgram(m_DefaultShaderProgram.GetID());
}

void GLFW_EngineCore::SetupDefaultFont() {
	// FreeType.
	FT_Library ft;

	// All functions return a value different to 0, whenever an error occurred.
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library." << std::endl;

	// Load font as face.
	FT_Face face;
	if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font." << std::endl;
		return;
	}

	// Set size to load glyphs as.
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set.
	for (GLubyte c = 0; c < 128; c++) {
		// Load the character glyph.
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph." << std::endl;
			continue;
		}

		// Generate a texture.
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set the texture options.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use.
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		m_Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we're finished.
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads.
	glGenVertexArrays(1, &m_Font_VAO);
	glGenBuffers(1, &m_Font_VBO);
	glBindVertexArray(m_Font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_Font_VBO);

	// Dynamic draw as the text may change frequently.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void GLFW_EngineCore::SetCamera(const std::shared_ptr<CameraComponent> p_Camera) {
	// Set the view and projection components of our shader to the camera values.
	glm::mat4 projection = glm::perspective(glm::radians(p_Camera->m_FieldOfView), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
	//glUniformMatrix4fv(glGetUniformLocation(m_DefaultShaderProgram.GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	m_DefaultShaderProgram.SetMat4("projection", projection);
	m_DefaultShaderProgram.SetMat4("view", p_Camera->GetViewMatrix());
	//glUniformMatrix4fv(glGetUniformLocation(m_DefaultShaderProgram.GetID(), "view"), 1, GL_FALSE, glm::value_ptr(p_Camera->GetViewMatrix()));

	// Be sure to activate shader when setting uniforms/drawing objects.
	m_DefaultShaderProgram.SetVec3("objectColour", 1.0f, 0.6f, 0.61f);
	m_DefaultShaderProgram.SetVec3("lightColour", 1.0f, 1.0f, 1.0f);
	m_DefaultShaderProgram.SetVec3("lightPos", 0.0f, 2.0f, -2.0f);
	m_DefaultShaderProgram.SetVec3("viewPos", p_Camera->Position());
	/*glUniform3f(glGetUniformLocation(m_DefaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_DefaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_DefaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_DefaultShaderProgram, "viewPos"), 1, glm::value_ptr(p_Camera->Position()));*/

}

void GLFW_EngineCore::DrawModel(std::shared_ptr<Model> p_Model, const glm::mat4 &p_ModelMatrix) {
	//glUniformMatrix4fv(glGetUniformLocation(m_DefaultShaderProgram.GetID(), "model"), 1, GL_FALSE, glm::value_ptr(p_ModelMatrix));
	m_DefaultShaderProgram.SetMat4("model", p_ModelMatrix);

	p_Model->Render(m_DefaultShaderProgram.GetID());
}

// The screen is between 0 and 1 with (0,0) at the bottom left corner.
void GLFW_EngineCore::RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour) {
	// Set the window to orthographic.
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_ScreenWidth), 0.0f, static_cast<float>(m_ScreenHeight));

	float pixelValueX = p_XPosition * m_ScreenWidth;
	float pixelValueY = p_YPosition * m_ScreenHeight;

	glUseProgram(m_FontShaderProgram.GetID());
	//glUniformMatrix4fv(glGetUniformLocation(m_FontShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	m_FontShaderProgram.SetMat4("projection", projection);

	// Activate corresponding render state.
	//glUniform3f(glGetUniformLocation(m_FontShaderProgram, "textColour"), p_Colour.x, p_Colour.y, p_Colour.z);
	m_FontShaderProgram.SetVec3("textColour", p_Colour.x, p_Colour.y, p_Colour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_Font_VAO);

	// Iterate through all characters.
	std::string::const_iterator c;
	for (c = p_Text.begin(); c != p_Text.end(); c++) {
		Character ch = m_Characters[*c];

		GLfloat xpos = pixelValueX + ch.m_Bearing.x * p_Scale;
		GLfloat ypos = pixelValueY - (ch.m_Size.y - ch.m_Bearing.y) * p_Scale;

		GLfloat w = ch.m_Size.x * p_Scale;
		GLfloat h = ch.m_Size.y * p_Scale;

		// Update VBO for each character.
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// Render glyph texture over quad.
		glBindTexture(GL_TEXTURE_2D, ch.m_TextureID);

		// Update content of VBO memory.
		glBindBuffer(GL_ARRAY_BUFFER, m_Font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels).
		pixelValueX += (ch.m_Advance >> 6) * p_Scale; // Bitshift by 6 to get value in pixels (2^6 = 64).
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Go back to default shader.
	glUseProgram(m_DefaultShaderProgram.GetID());
}