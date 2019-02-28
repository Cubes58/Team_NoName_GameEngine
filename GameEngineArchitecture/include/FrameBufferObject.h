#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "CubemapCamera.h"
#include "FrameBufferType.h"


class RenderEngine;

class FrameBufferObject
{

private:

	int m_FrameWidth, m_FrameHeight; //!< Width and height of the FBO
	GLuint m_FrameBuffer; //!< ID of the framebuffer

	bool m_MultiSampledTarget = false; //!< For Multi textured objects

	unsigned int m_ColourTexture; //!< Colour texture of the scene
	unsigned int m_ColourBuffer; //!< Colour buffer list for rendering (adapted for multi targetting)

	unsigned int m_DepthTexture; //!< Depth of the scene
	unsigned int m_DepthBuffer; //!< Buffer of the depth for rendering

	unsigned int m_FrameBufferTextureID; //!< Texture ID

	CubeMapCamera m_CubeMapCamera; //!< Camera for generating cubemaps
	RenderEngine *m_RenderEngine = nullptr; //!< Pointer to the engine for rendering 
	FrameBufferType m_TypeIndicator;


	void InitFrameBuffer(); //!< Initialises the framebuffer
	void CreateFrameBuffer(); //!< Creates the framebuffer object;
	void CreateTextureAttachment(); //!< Creates and adds the texture attachment (Colour)
	void CreateDepthTextureAttachment(); //!< Creates and adds the texture attachment (Depth)
	void CreateDepthBufferAttachment(); //!< Creates and adds the buffer for rendering


public:
	static const constexpr int NONE = 0;
	static const constexpr int DEPTH_TEXTURE = 1;
	static const constexpr int DEPTH_RENDER_BUFFER = 2;

	FrameBufferObject(int p_Width, int p_Height, FrameBufferType p_Type);
	FrameBufferObject(RenderEngine *p_RenderEngine, glm::vec3 p_Position, float p_Size);

	void CleanUp(); //!< Clean up the FBOs after destruction
	void BindFrameBuffer(); //!< Bind the frame buffer to the current context
	void UnbindFrameBuffer(); //!< Unbinds the frame buffer from the current context, returns it to screen

	void BindToReadBuffer(); //!< Binds the current frame buffer to a read buffer
	void ResolveToFrameBuffer(int p_ReadBuffer, FrameBufferObject p_OutputFrameBuffer); //!< Passer between multiple frame buffers
	void ResolveToScreenBuffer(); //!< Resolves current frame buffer to the screen
	void FrameBufferCubeMapping(); //!< Frame buffer for cube mapping textures - uses camera and records surroundings

	int GetColourTexture(); //!< Returns the colour texture
	int GetDepthTexture(); //!< Returns the depth texture
	int GetTextureID(); //!< Returns the Frame buffers texture ID




};