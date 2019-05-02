#include "ImageRenderer.h"
#include "FrameBufferType.h"

ImageRenderer::ImageRenderer()
{

}

ImageRenderer::ImageRenderer(int p_Width, int p_Height)
{
	m_FBO = new FrameBufferObject(p_Width, p_Height, FrameBufferType::NONE);
}

void ImageRenderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

int ImageRenderer::GetOutputTexture()
{
	return m_FBO->GetColourTexture();
}

