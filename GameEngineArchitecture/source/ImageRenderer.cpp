#include "ImageRenderer.h"

ImageRenderer::ImageRenderer()
{

}

ImageRenderer::ImageRenderer(int p_Width, int p_Height)
{
	m_FBO = new FrameBufferObject(p_Width, p_Height, 0);
}

void ImageRenderer::Render()
{
	if (m_FBO != NULL)
	{
		m_FBO->BindFrameBuffer();
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (m_FBO != nullptr)
	{
		m_FBO->UnbindFrameBuffer();
	}
}

int ImageRenderer::GetOutputTexture()
{
	return m_FBO->GetColourTexture();
}

