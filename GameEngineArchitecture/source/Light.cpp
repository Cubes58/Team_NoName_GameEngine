#include "Light.h"

void Light::InitDebugModel()
{
	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};
	glGenVertexArrays(1, &m_VaoHandler);
	glGenBuffers(1, &m_VboHandler);

	glBindVertexArray(m_VaoHandler);

	glBindBuffer(GL_ARRAY_BUFFER, m_VboHandler);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GenerateModelMatrix();
}

void Light::GenerateModelMatrix()
{
	glm::mat4 l_TransformMatrix = glm::translate(glm::mat4(1.0f), m_LightPosition);
	glm::mat4 l_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 l_RotationMatrix = glm::mat4(1.0f);
	m_LightModelMatrix = l_TransformMatrix * l_RotationMatrix * l_ScaleMatrix;
}

void Light::GenerateViewMatrix()
{
	glm::vec3 l_Front;
	l_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	l_Front.y = sin(glm::radians(m_Pitch));
	l_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_FrontVector = glm::normalize(l_Front);

	m_RightVector = glm::normalize(glm::cross(m_FrontVector, glm::vec3(0, 1, 0)));
	m_UpVector = glm::normalize(glm::cross(m_RightVector, m_FrontVector));

	m_LightViewMatrix = glm::lookAt(m_LightPosition, m_LightPosition + m_FrontVector, m_UpVector);
}


Light::Light()
{
	m_LightPosition = glm::vec3(0.0f, 50.0f, 0.0f);
	m_LightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Pitch = 0.0f;
	m_Yaw = -90.0f;
	GenerateModelMatrix();
	InitDebugModel();
}

Light::Light(glm::vec3 p_LightPosition, glm::vec3 p_LightColour)
{
	m_LightPosition = p_LightPosition;
	m_LightColour = p_LightColour;
	m_Pitch = 0.0f;
	m_Yaw = -90.0f;
	GenerateModelMatrix();
	GenerateViewMatrix();
	InitDebugModel();
}

Light::Light(glm::vec3 p_LightPosition, glm::vec3 p_LightColour, float p_Pitch, float p_Yaw)
{
	m_LightPosition = p_LightPosition;
	m_LightColour = p_LightColour;
	m_Pitch = p_Pitch;
	m_Yaw = p_Yaw;
	GenerateModelMatrix();
	GenerateViewMatrix();
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

void Light::SetRotation(float p_Pitch, float p_Yaw)
{
	m_Pitch = p_Pitch;
	m_Yaw = p_Yaw;
	GenerateViewMatrix();
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

const glm::mat4 &Light::GetViewMatrix()
{
	//GenerateViewMatrix();
	return m_LightViewMatrix;
}
