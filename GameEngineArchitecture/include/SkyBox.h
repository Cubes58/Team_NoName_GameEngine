#pragma once
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <memory>
#include <STB_IMAGE/stb_image.h>
#include "ShaderProgram.h"




class Skybox
{
private:

	std::shared_ptr<ShaderProgram> m_ShaderProgram;
	unsigned int m_VaoHandler;
	unsigned int m_VboHandler;

	unsigned int m_TextureID;

	float m_SkyboxSize;

	std::vector<std::string> m_TextureList;
	std::vector<unsigned int> m_SkyboxOrientation;

	GLuint createTexture(std::vector<std::string> p_TextureList);
	void bindTextures();

public:


	Skybox(float);

	void Render();
	void Update(float p_DeltaTime);
	void SetParams(glm::mat4 p_Projection, glm::mat4 p_ViewMatrix);
	
	int GetTextureID();
	std::shared_ptr<ShaderProgram> GetShaderProgram();



};