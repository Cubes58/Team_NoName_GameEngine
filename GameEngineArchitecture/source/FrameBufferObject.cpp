#include "FrameBufferObject.h"

void FrameBufferObject::InitFrameBuffer()
{
	CreateFrameBuffer();
	CreateTextureAttachment();

	if (m_TypeIndicator == FrameBufferType::DEPTH_RENDER_BUFFER)
	{
		CreateDepthBufferAttachment();
	}

	else if (m_TypeIndicator == FrameBufferType::DEPTH_TEXTURE)
	{
		CreateDepthTextureAttachment();
	}
}

void FrameBufferObject::CreateFrameBuffer()
{
	glGenFramebuffers(1, &m_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBufferObject::CreateTextureAttachment()
{
	glGenTextures(1, &m_ColourTexture);
	glBindTexture(GL_TEXTURE_2D, m_ColourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FrameWidth, m_FrameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte *)NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourTexture, 0);
}

void FrameBufferObject::CreateDepthTextureAttachment()
{
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_FrameWidth, m_FrameHeight, 0, GL_DEPTH_COMPONENT,
		GL_FLOAT, (GLubyte*)NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
}

void FrameBufferObject::CreateDepthBufferAttachment()
{
	glGenRenderbuffers(1, &m_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_FrameWidth, m_FrameHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

	
}

FrameBufferObject::FrameBufferObject(int p_Width, int p_Height, FrameBufferType p_Type)
{
	m_FrameWidth = p_Width;
	m_FrameHeight = p_Height;
	m_TypeIndicator = p_Type;

	InitFrameBuffer();
	UnbindFrameBuffer();
}

FrameBufferObject::FrameBufferObject(RenderEngine *p_RenderEngine, glm::vec3 p_Position, float p_Size)
{
	m_RenderEngine = (static_cast<RenderEngine*>(p_RenderEngine));
	m_CubeMapCamera.SetPosition(p_Position);

	m_FrameWidth = p_Size;
	m_FrameHeight = p_Size;


	glGenTextures(1, &m_FrameBufferTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_FrameBufferTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Allocate space for each side of the cube map
	for (GLuint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_FrameWidth, 
			         m_FrameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &m_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glGenRenderbuffers(1, &m_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_FrameWidth, m_FrameHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void FrameBufferObject::CleanUp()
{
	glDeleteFramebuffers(1, &m_FrameBuffer);
	glDeleteTextures(1, &m_ColourTexture);
	glDeleteTextures(1, &m_DepthTexture);
	glDeleteTextures(1, &m_FrameBufferTextureID);

	glDeleteRenderbuffers(1, &m_DepthBuffer);
	glDeleteRenderbuffers(1, &m_ColourBuffer);

}

void FrameBufferObject::BindFrameBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBuffer);
	glViewport(0, 0, m_FrameWidth, m_FrameHeight);
}

void FrameBufferObject::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800, 600);
}

void FrameBufferObject::BindToReadBuffer()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBufferObject::FrameBufferCubeMapping()
{
	glViewport(0.0, 0.0, m_FrameWidth, m_FrameHeight);

	for (int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			m_FrameBufferTextureID, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		m_CubeMapCamera.SwitchToFace(i);
		m_RenderEngine->RenderFrameBuffers(m_CubeMapCamera);
	}
}

int FrameBufferObject::GetColourTexture()
{
	return m_ColourTexture;
}

int FrameBufferObject::GetDepthTexture()
{
	return m_DepthTexture;
}

int FrameBufferObject::GetTextureID()
{
	return m_FrameBufferTextureID;
}
