#include "VBOQuad.h"

VBOQuad::VBOQuad()
{
	float l_Points[] =
	{
		-1, 1, -1, -1, 1, 1, 1, -1
	};

	glGenVertexArrays(1, &m_VaoHandle);
	glGenBuffers(1, &m_VboHandle);
	glBindVertexArray(m_VaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboHandle);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GL_FLOAT), &l_Points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void VBOQuad::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_VaoHandle);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int VBOQuad::GetVaoHandle()
{
	return m_VaoHandle;
}
