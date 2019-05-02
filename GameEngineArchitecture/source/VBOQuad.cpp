#include "VBOQuad.h"

VBOQuad::VBOQuad()
{
	float l_Points[] =
	{
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &m_VaoHandle);
	glGenBuffers(1, &m_VboHandle);
	glBindVertexArray(m_VaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(l_Points), &l_Points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void VBOQuad::Render()
{
	glBindVertexArray(m_VaoHandle);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int VBOQuad::GetVaoHandle()
{
	return m_VaoHandle;
}
