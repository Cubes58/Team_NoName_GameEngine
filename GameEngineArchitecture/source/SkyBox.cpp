#include "SkyBox.h"


Skybox::Skybox(float p_SkyboxSize)
{
	this->m_SkyboxSize = p_SkyboxSize;


	float points[] = {
		-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
		-m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
		-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,

		-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
		-m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
		-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
		-m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
		-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,

		m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,

		-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
		-m_SkyboxSize, m_SkyboxSize,   m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize,   m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize,   m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
		-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,

		-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
		m_SkyboxSize, m_SkyboxSize,  m_SkyboxSize,
		-m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
		-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,

		-m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
		-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
		m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize
	};

	glGenVertexArrays(1, &m_VaoHandler);
	glGenBuffers(1, &m_VboHandler);
	glBindVertexArray(m_VaoHandler);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboHandler);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GL_FLOAT), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

	m_TextureList.push_back("resources/textures/skybox/ssNY.bmp");
	m_TextureList.push_back("resources/textures/skybox/ssNZ.bmp");
	m_TextureList.push_back("resources/textures/skybox/ssNX.bmp");
	m_TextureList.push_back("resources/textures/skybox/ssPY.bmp");
	m_TextureList.push_back("resources/textures/skybox/ssPZ.bmp");
	m_TextureList.push_back("resources/textures/skybox/ssPX.bmp");

	m_SkyboxOrientation.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_SkyboxOrientation.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	m_SkyboxOrientation.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_SkyboxOrientation.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_SkyboxOrientation.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_SkyboxOrientation.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_X);

	m_TextureID = createTexture(m_TextureList);

	m_ShaderProgram = std::make_shared<ShaderProgram>();
	m_ShaderProgram->CompileShader("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");


}

GLuint Skybox::createTexture(std::vector<std::string> &p_TextureList)
{
	GLuint l_textureID;
	glGenTextures(1, &l_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, l_textureID);

	int imageWidth, imageHeight, nrComponents;
	unsigned char* textureData;

	for (GLuint i = 0; i < p_TextureList.size(); i++)
	{
		textureData = stbi_load(p_TextureList[i].c_str(), &imageWidth, &imageHeight, &nrComponents, 0);

		if (textureData)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glTexImage2D(m_SkyboxOrientation[i], 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, textureData);
		}
		else
		{
			std::cout << "Texture failed to load from: " << p_TextureList[i] << std::endl;
		}
		stbi_image_free(textureData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return l_textureID;
}

void Skybox::bindTextures()
{
	glUseProgram(m_ShaderProgram->GetID());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

	m_ShaderProgram->SetInt("skybox", 0);
}


void Skybox::Render()
{
	bindTextures();
	glBindVertexArray(m_VaoHandler);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

int Skybox::GetTextureID()
{
	return m_TextureID;
}

std::shared_ptr<ShaderProgram> Skybox::GetShaderProgram()
{
	return m_ShaderProgram;
}
