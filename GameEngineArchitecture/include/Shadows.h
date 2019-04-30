#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <memory>

#include <FrameBufferObject.h>
#include <FrameBufferType.h>
#include <ShaderProgram.h>


class Shadows
{
private:
	std::shared_ptr<FrameBufferObject> m_ShadowBuffer;
	std::shared_ptr<ShaderProgram> m_ShadowDepthShader;
	unsigned int m_Width;
	unsigned int m_Height;
	bool m_IsDirectional;

public:
	Shadows(float p_Width, float p_Height, bool p_IsDirectional);

	void Prepare(glm::vec3 p_SunPosition, float p_NearClip, float p_FarClip);
	std::shared_ptr<ShaderProgram> Render();
	void End(unsigned int p_Width, unsigned int p_Height);
	unsigned int GetShadowMap();
};