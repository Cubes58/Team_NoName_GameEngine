#pragma once
#include "ShaderProgram.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H


class FontRenderer
{

private:

	FontRenderer(std::string &p_FontPath, int p_ScreenWidth, int p_ScreenHeight, GLuint p_DefaultShaderID);

	int m_ScreenWidth;
	int m_ScreenHeight;

	/**
	* A structure to represent a Character.
	*/
	struct Character {
		GLuint m_TextureID;		// ID handle of the glyph texture.
		glm::ivec2 m_Size;		// Size of the glyph.
		glm::ivec2 m_Bearing;	// Offset from baseline to left/top of the glyph.
		GLuint m_Advance;		// Horizontal offset to advance to next glyph.
	};
	std::map<GLchar, Character> m_Characters;	//!< A map of the characters.
	GLuint m_Font_VBO;	//!< The font vertex buffer object.
	GLuint m_Font_VAO;	//!< The font vertex array object.

	std::string m_FontPath;

	ShaderProgram m_FontShaderProgram; //!< Shader for Font
	GLuint m_DefaultShaderID; //!< ID for the default shader to revert too

	/*!
		\brief initialises the font.
	*/
	void InitFont();

public:

	void SetShader(const GLchar *p_VertexPath, const GLchar *p_FragmentPath);

	/*!
	\brief Used to render text.
	\param p_Text the text to be displayed.
	\param p_XPosition the screen's X position, of where the text should be rendered.
	\param p_YPosition the screen's Y position, of where the text should be rendered.
	\param p_Scale the text's scale.
	\param p_Colour the colour of the text.
	*/
	void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, glm::vec3 p_Colour);

	void SetFont(const std::string &p_FontPath);
};
