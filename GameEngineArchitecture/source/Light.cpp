#include "Light.h"

void Light::InitDebugModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	
	glGenVertexArrays(1, &m_VaoHandler);
	glGenBuffers(1, &m_VboHandler);

	glBindVertexArray(m_VaoHandler);

	glBindBuffer(GL_ARRAY_BUFFER, m_VboHandler);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GenerateModelMatrix();
}

void Light::GenerateModelMatrix()
{
	glm::mat4 l_TransformMatrix = glm::translate(glm::mat4(1.0f), m_LightPosition);
	glm::mat4 l_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 l_RotationMatrix = glm::mat4(1.0f);
	m_LightModelMatrix = l_TransformMatrix * l_RotationMatrix * l_ScaleMatrix;
}

void Light::GenerateProjectionMatrix(glm::vec2 p_ClipPlanes, float p_AspectRatio, float p_FieldOfView, float p_FrustrumLength)
{
	m_LightProjectionMatrix = glm::mat4();
	float l_YScale = (float)((1.0f / glm::tan(glm::radians(p_FieldOfView / 2.f))));
	float l_XScale = l_YScale / p_AspectRatio;

	m_LightProjectionMatrix[0][0] = l_XScale;
	m_LightProjectionMatrix[1][1] = l_YScale;
	m_LightProjectionMatrix[2][2] = -((p_ClipPlanes.y + p_ClipPlanes.x) / p_FrustrumLength);
	m_LightProjectionMatrix[2][3] = -1;
	m_LightProjectionMatrix[3][2] = -((2 * p_ClipPlanes.x * p_ClipPlanes.y) / p_FrustrumLength);
	m_LightProjectionMatrix[3][3] = 0;
}

Light::Light()
{
	m_LightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	m_LightColour = glm::vec3(1.0f, 1.0f, 1.0f);

	InitDebugModel();
}

Light::Light(glm::vec3 p_LightPosition, glm::vec3 p_LightColour)
{
	m_LightPosition = p_LightPosition;
	m_LightColour = p_LightColour;

	InitDebugModel();
}

void Light::Render()
{
	glBindVertexArray(m_VaoHandler);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Light::SetPosition(glm::vec3 p_LightPosition)
{
	m_LightPosition = p_LightPosition;
	GenerateModelMatrix();
}

void Light::SetPosition(float p_X, float p_Y, float p_Z)
{
	m_LightPosition = glm::vec3(p_X, p_Y, p_Z);
	GenerateModelMatrix();
}

void Light::SetColour(glm::vec3 p_LightColour)
{
	m_LightColour = p_LightColour;
}

void Light::SetColour(float p_Red, float p_Green, float p_Blue)
{
	m_LightColour = glm::vec3(p_Red, p_Green, p_Blue);
}

void Light::SetProjectionMatrix(glm::vec2 p_ClipPlanes, float p_AspectRatio, float p_FieldOfView, float p_FrustrumLength)
{
	GenerateProjectionMatrix(p_ClipPlanes, p_AspectRatio, p_FieldOfView, p_FrustrumLength);
}

const glm::vec3 & Light::GetPosition() const
{
	return m_LightPosition;
}

const glm::vec3 & Light::GetColour() const
{
	return m_LightColour;
}

const glm::mat4 & Light::GetModelMatrix() const
{
	return m_LightModelMatrix;
}

const glm::mat4 &Light::GetSpaceMatrix()
{
	m_LightViewMatrix = glm::lookAt(m_LightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return glm::mat4(m_LightProjectionMatrix * m_LightViewMatrix);
}
