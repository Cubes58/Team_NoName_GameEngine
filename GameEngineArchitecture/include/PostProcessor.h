#pragma once
#include "ContrastChanger.h"
#include "VBOQuad.h"



class PostProcessor
{
private:
	VBOQuad m_ScreenQuad;
	ContrastChanger* m_ContrastChanger;

	void Start();
	void End();

public:
	void InitPostProcessing();
	void DoPostProcessing(int p_ColourTexture);
};