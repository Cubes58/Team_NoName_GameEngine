#pragma once
#include "FrameBufferObject.h"



class ImageRenderer
{
private:
	FrameBufferObject *m_FBO;

public:
	ImageRenderer();
	ImageRenderer(int p_Width, int p_Height);

	void Render();

	int GetOutputTexture();


};