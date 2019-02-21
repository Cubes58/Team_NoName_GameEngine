#include "ShaderProgram.h"
#include <sstream>
#include <fstream>



ShaderProgram::ShaderProgram()
{

}

void ShaderProgram::CompileShader(const GLchar *p_VertexPath, const GLchar *p_FragmentPath)
{
	std::string l_VertexCode;
	std::string l_FragmentCode;

	std::ifstream l_VertexFile;
	std::ifstream l_FragmentFile;

	// Ensure ifstream objects can throw exceptions:
	l_VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	l_FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open files.
		l_VertexFile.open(p_VertexPath);
		l_FragmentFile.open(p_FragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// Read file's buffer contents into streams.
		vShaderStream << l_VertexFile.rdbuf();
		fShaderStream << l_FragmentFile.rdbuf();

		// Close file handlers.
		l_VertexFile.close();
		l_FragmentFile.close();

		// Convert stream into string.
		l_VertexCode = vShaderStream.str();
		l_FragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
	}

	const char *l_VertShaderCode = l_VertexCode.c_str();
	const char *l_FragShaderCode = l_FragmentCode.c_str();

	// Compile the shaders.
	unsigned int l_Vertex, l_Fragment;

	// Vertex Shader:
	l_Vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(l_Vertex, 1, &l_VertShaderCode, nullptr);
	glCompileShader(l_Vertex);

	// Print compile errors, if any.
	CheckErrors(l_Vertex, "VERTEX");
	

	// Fragment Shader:
	l_Fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(l_Fragment, 1, &l_FragShaderCode, nullptr);
	glCompileShader(l_Fragment);

	// Print compile errors, if any.
	CheckErrors(l_Vertex, "FRAGMENT");

	// Link both shaders, to create a shader program.
	m_ID = glCreateProgram();
	glAttachShader(m_ID, l_Vertex);
	glAttachShader(m_ID, l_Fragment);
	glLinkProgram(m_ID);

	// Print linking errors, if any.
	CheckErrors(m_ID, "PROGRAM");

	glDeleteShader(l_Vertex);
	glDeleteShader(l_Fragment);
}

void ShaderProgram::CompileShader(const GLchar *p_VertexPath, const GLchar *p_FragmentPath, const GLchar *p_GeometryPath)
{
	std::string l_VertexCode;
	std::string l_FragmentCode;
	std::string l_GeometryCode;

	std::ifstream l_VertexFile;
	std::ifstream l_FragmentFile;
	std::ifstream l_GeometryFile;

	// Ensure ifstream objects can throw exceptions:
	l_VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	l_FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	l_GeometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open files.
		l_VertexFile.open(p_VertexPath);
		l_FragmentFile.open(p_FragmentPath);
		l_GeometryFile.open(p_GeometryPath);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		// Read file's buffer contents into streams.
		vShaderStream << l_VertexFile.rdbuf();
		fShaderStream << l_FragmentFile.rdbuf();
		gShaderStream << l_GeometryFile.rdbuf();

		// Close file handlers.
		l_VertexFile.close();
		l_FragmentFile.close();
		l_GeometryFile.close();

		// Convert stream into string.
		l_VertexCode = vShaderStream.str();
		l_FragmentCode = fShaderStream.str();
		l_GeometryCode = gShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
	}

	const char *l_VertShaderCode = l_VertexCode.c_str();
	const char *l_FragShaderCode = l_FragmentCode.c_str();
	const char *l_GeoShaderCode = l_GeometryCode.c_str();

	// Compile the shaders.
	unsigned int l_Vertex, l_Fragment, l_Geometry;

	// Vertex Shader:
	l_Vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(l_Vertex, 1, &l_VertShaderCode, nullptr);
	glCompileShader(l_Vertex);

	// Print compile errors, if any.
	CheckErrors(l_Vertex, "VERTEX");


	// Fragment Shader:
	l_Fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(l_Fragment, 1, &l_FragShaderCode, nullptr);
	glCompileShader(l_Fragment);

	// Print compile errors, if any.
	CheckErrors(l_Vertex, "FRAGMENT");

	// Geometry Shader:
	l_Geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(l_Geometry, 1, &l_GeoShaderCode, nullptr);
	glCompileShader(l_Geometry);

	// Link shaders, to create a shader program.
	m_ID = glCreateProgram();
	glAttachShader(m_ID, l_Vertex);
	glAttachShader(m_ID, l_Fragment);
	glAttachShader(m_ID, l_Geometry);
	glLinkProgram(m_ID);

	// Print linking errors, if any.
	CheckErrors(m_ID, "PROGRAM");

	glDeleteShader(l_Vertex);
	glDeleteShader(l_Fragment);
	glDeleteShader(l_Geometry);
}

void ShaderProgram::ErrorChecker()
{
	GLenum e;
	while ((e = glGetError()) != GL_NO_ERROR) {
		switch (e) {
		case GL_INVALID_ENUM:
			std::cout << "\nERROR: GL_INVALID_ENUM\n";
			assert(false);
			break;
		case GL_INVALID_VALUE:
			std::cout << "\nERROR: GL_INVALID_VALUE\n";
			assert(false);
			break;
		case GL_INVALID_OPERATION:
			std::cout << "\nERROR: GL_INVALID_OPERATION\n";
			assert(false);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "\nERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n";
			assert(false);
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "\nERROR: GL_OUT_OF_MEMORY\n";
			assert(false);
			break;
		default:
			std::cout << "\nERROR: UNKNOWN\n";
			assert(false);
			break;
		}
	}
}

void ShaderProgram::CreateShader(GLuint &p_ShaderID, const GLenum &p_ShaderType, const GLchar *p_ShaderSource, const std::string &p_TypeInformation)
{
	//Errors currently occur when calling this function - needs to be looked at - Tom 14/02/19
	std::cout << "\nSHADER SOURCE CODE:" << p_ShaderType << " " << "SHADER_TYPE: " << p_TypeInformation << "\n" << p_ShaderSource;

	p_ShaderID = glCreateShader(p_ShaderType);

	glShaderSource(p_ShaderID, 1, &p_ShaderSource, nullptr);
	glCompileShader(p_ShaderID);
	CheckErrors(p_ShaderID, p_TypeInformation);
}

void ShaderProgram::CheckErrors(GLuint p_Object, const std::string &p_Type)
{
	GLint l_Success;
	GLchar l_InfoLog[1024];

	if (p_Type != "PROGRAM") {
		glGetShaderiv(p_Object, GL_COMPILE_STATUS, &l_Success);
		if (!l_Success) {
			glGetShaderInfoLog(p_Object, 1024, NULL, l_InfoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << p_Type << "\n" << l_InfoLog;
		}
	}
	else {
		glGetProgramiv(p_Object, GL_LINK_STATUS, &l_Success);
		if (!l_Success) {
			glGetProgramInfoLog(p_Object, 1024, NULL, l_InfoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << p_Type << "\n" << l_InfoLog;
		}
	}
}

ShaderProgram &ShaderProgram::Use()
{
	glUseProgram(m_ID);
	return *this;
}

void ShaderProgram::SetBool(const std::string & p_Name, bool p_Value) const
{
	glUniform1i(glGetUniformLocation(m_ID, p_Name.c_str()), static_cast<GLint>(p_Value));
}

void ShaderProgram::SetInt(const std::string & p_Name, int p_Value) const
{
	glUniform1i(glGetUniformLocation(m_ID, p_Name.c_str()), p_Value);
}

void ShaderProgram::SetFloat(const std::string & p_Name, float p_Value) const
{
	glUniform1f(glGetUniformLocation(m_ID, p_Name.c_str()), p_Value);
}

void ShaderProgram::SetVec2(const std::string & p_Name, const glm::vec2 & p_Value) const
{
	glUniform2fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}

void ShaderProgram::SetVec2(const std::string & p_Name, float p_XValue, float p_YValue) const
{
	glUniform2f(glGetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue);
}

void ShaderProgram::SetVec3(const std::string & p_Name, const glm::vec3 & p_Value) const
{
	glUniform3fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}

void ShaderProgram::SetVec3(const std::string & p_Name, float p_XValue, float p_YValue, float p_ZValue) const
{
	glUniform3f(glGetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue, p_ZValue);
}

void ShaderProgram::SetVec4(const std::string & p_Name, const glm::vec4 & p_Value) const
{
	glUniform4fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}

void ShaderProgram::SetVec4(const std::string & p_Name, float p_XValue, float p_YValue, float p_ZValue, float p_WValue) const
{
	glUniform4f(glGetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue, p_ZValue, p_WValue);
}

void ShaderProgram::SetMat2(const std::string & p_Name, const glm::mat2 & p_Mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, GL_FALSE, glm::value_ptr(p_Mat));
}

void ShaderProgram::SetMat3(const std::string & p_Name, const glm::mat3 & p_Mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, GL_FALSE, glm::value_ptr(p_Mat));
}

void ShaderProgram::SetMat4(const std::string & p_Name, const glm::mat4 & p_Mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, GL_FALSE, glm::value_ptr(p_Mat));
}

unsigned int ShaderProgram::GetID() const
{
	return m_ID;
}

void ShaderProgram::SetID(unsigned int p_ID)
{
	m_ID = p_ID;
}


