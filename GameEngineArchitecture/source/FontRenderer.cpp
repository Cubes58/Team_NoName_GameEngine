#include "FontRenderer.h"
#include <glm/gtc/matrix_transform.hpp>



FontRenderer::FontRenderer(const std::string & p_FontPath, int p_ScreenWidth, int p_ScreenHeight, GLuint p_DefaultShaderID)
{
	m_FontPath = p_FontPath;
	m_ScreenWidth = p_ScreenWidth;
	m_ScreenHeight = p_ScreenHeight;
	m_DefaultShaderID = p_DefaultShaderID;

	InitFont();
}


void FontRenderer::InitFont()
{
	
	// FreeType.
	FT_Library ft;

	// All functions return a value different to 0, whenever an error occurred.
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library." << std::endl;

	// Load font as face.
	FT_Face face;
	if (FT_New_Face(ft, m_FontPath.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font." << std::endl;
		return;
	}

	// Set size to load glyphs as.
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set.
	for (GLubyte c = 0; c < 128; c++) {
		// Load the character glyph.
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph." << std::endl;
			continue;
		}

		// Generate a texture.
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set the texture options.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use.
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		m_Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we're finished.
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads.
	glGenVertexArrays(1, &m_Font_VAO);
	glGenBuffers(1, &m_Font_VBO);
	glBindVertexArray(m_Font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_Font_VBO);

	// Dynamic draw as the text may change frequently.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FontRenderer::SetShader(const GLchar * p_VertexPath, const GLchar * p_FragmentPath)
{
	m_FontShaderProgram.CompileShader(p_VertexPath, p_FragmentPath);
}

void FontRenderer::RenderText(const std::string & p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_ScreenWidth), 0.0f, static_cast<float>(m_ScreenHeight));

	float pixelValueX = p_XPosition * m_ScreenWidth;
	float pixelValueY = p_YPosition * m_ScreenHeight;

	//Enable Blending for the text gaps
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(m_FontShaderProgram.GetID());
	m_FontShaderProgram.SetMat4("projection", projection);

	// Activate corresponding render state.
	m_FontShaderProgram.SetVec3("textColour", p_Colour);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_Font_VAO);

	// Iterate through all characters.
	std::string::const_iterator c;
	for (c = p_Text.begin(); c != p_Text.end(); c++) {
		Character ch = m_Characters[*c];

		GLfloat xpos = pixelValueX + ch.m_Bearing.x * p_Scale;
		GLfloat ypos = pixelValueY - (ch.m_Size.y - ch.m_Bearing.y) * p_Scale;

		GLfloat w = ch.m_Size.x * p_Scale;
		GLfloat h = ch.m_Size.y * p_Scale;

		// Update VBO for each character.
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// Render glyph texture over quad.
		glBindTexture(GL_TEXTURE_2D, ch.m_TextureID);

		// Update content of VBO memory.
		glBindBuffer(GL_ARRAY_BUFFER, m_Font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels).
		pixelValueX += (ch.m_Advance >> 6) * p_Scale; // Bitshift by 6 to get value in pixels (2^6 = 64).
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Go back to default shader.
	glUseProgram(m_DefaultShaderID);
}

void FontRenderer::SetFont(const std::string &p_FontPath)
{
	m_FontPath = p_FontPath;
	InitFont();
}
