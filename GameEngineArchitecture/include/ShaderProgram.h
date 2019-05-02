#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <functional>
#include <string>

#include <iostream>


class ShaderProgram
{
private:
	unsigned int m_ID; //!< Shader Program ID


	void CreateShader(GLuint &p_ShaderID, const GLenum &p_ShaderType, const GLchar *p_ShaderSource, const std::string &p_TypeInf = "Shader");
	void CheckErrors(GLuint p_Object, const std::string &p_Type);

public:

	ShaderProgram();

	// Compiles the shader using the paths specified - adaptable for Geometry and Tessellation
	void CompileShader(const GLchar *p_VertexPath, const GLchar *p_FragmentPath);
	void CompileShader(const GLchar *p_VertexPath, const GLchar *p_FragmentPath, const GLchar *p_GeometryPath);

	// Error checker to be called every frame in update
	void ErrorChecker();

	//Uses the program (sets it as current program)
	ShaderProgram &Use();

	//Utility functions for sending information to shader
	void SetBool(const std::string &p_Name, bool p_Value) const;
	void SetInt(const std::string &p_Name, int p_Value) const;
	void SetFloat(const std::string &p_Name, float p_Value) const;

	void SetVec2(const std::string &p_Name, const glm::vec2 &p_Value) const;
	void SetVec2(const std::string &p_Name, float p_XValue, float p_YValue) const;

	void SetVec3(const std::string &p_Name, const glm::vec3 &p_Value) const;
	void SetVec3(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue) const;

	void SetVec4(const std::string &p_Name, const glm::vec4 &p_Value) const;
	void SetVec4(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue, float p_WValue) const;

	void SetMat2(const std::string &p_Name, const glm::mat2 &p_Mat) const;
	void SetMat3(const std::string &p_Name, const glm::mat3 &p_Mat) const;
	void SetMat4(const std::string &p_Name, const glm::mat4 &p_Mat) const;

	//Getter and Setter for the shader ID
	unsigned int GetID() const;
	void SetID(unsigned int p_ID);


};



