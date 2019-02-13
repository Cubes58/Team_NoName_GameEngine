/**
@file Mesh.h
@brief A class that deals with a model mesh.
*/
#pragma once

#include <glad/glad.h> 

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

/**
	* A structure to represent Vertex information.
*/
struct Vertex {
	glm::vec3 m_Position;	//!< Stores the position
	glm::vec3 m_Normal;	//!< Stores the normal.
	glm::vec2 m_TextureCoordinates;	//!< Stores the texture coordinates.
};

/**
	* A structure to represent Texture information.
*/
struct Texture {
	unsigned int m_ID;	//!< Stores the texture ID.
	std::string m_Type;	//!< Stores the type of texture.
	aiString p_FilePath; //!< stores the file path, to the texture.
};

/*! \class Mesh
	\brief A class that deals with a model mesh.
*/
class Mesh {
private:
	// Buffer objects.
	unsigned int m_VertexBufferObject;	//!< Stores an ID to the vertex buffer object.
	unsigned int m_ElementBufferObject;	//!< Stores an ID to the element buffer object.

	/*!
		\brief Initialises all the buffer arrays.
	*/
	void SetupMesh();

public:
	std::vector<Vertex> m_Vertices;		//!< Stores the vertices.
	std::vector<unsigned int> m_Indices;	//!< Stores the indices.
	std::vector<Texture> m_Textures;	//!< Stores the textures.
	unsigned int m_VertexArrayObject;	//!< Stores the vertex array objects.

	/*!
		\brief Constructor.
		\param p_Vertices the mesh's vertices.
		\param p_Indices the mesh's indices.
		\param p_Textures the mesh's textures.
	*/
	Mesh(std::vector<Vertex> p_Vertices, std::vector<unsigned int> p_Indices, std::vector<Texture> p_Textures);

	/*!
		\brief Render the mesh with a given shader program.
		\param p_ShaderProgram the shader program, used to render the mesh.
	*/
	void Render(const unsigned int p_ShaderProgram);
};