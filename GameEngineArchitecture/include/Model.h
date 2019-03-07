/**
@file Model.h
@brief A class that stores the properties necessary to create a model.
*/
#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>
#include <vector>

#include "Mesh.h"

/*! \class Model
	\brief A class that stores the properties necessary to create a model.
*/
class ModelComponent;

class Model {
private:
	friend ModelComponent;

	std::vector<Mesh> m_Meshes;	//!< Stores the model's meshes.
	std::string m_Directory;	//!< Stores the directory.
	std::vector<Texture> m_Textures;	//!< Stores the model's textures.

	/*!
		\brief Loads the model data.
		\param p_Path the file path to the model.
	*/
	bool LoadModel(std::string p_Path);
	/*!
		\brief Processes the model node.
		\param p_Node the ai node.
		\param p_Scene the ai scene.
	*/
	void ProcessNode(aiNode *p_Node, const aiScene *p_Scene);
	/*!
		\brief Processes the model mesh.
		\param p_Path the ai mesh.
		\param p_Scene the ai scene.
	*/
	Mesh ProcessMesh(aiMesh *p_Mesh, const aiScene *p_Scene);
	/*!
		\brief Loads the model material textures.
		\param p_Material the model's material.
		\param p_Type the texture type.
		\param p_TypeName the type name.
	*/
	std::vector<Texture> LoadMaterialTextures(aiMaterial *p_Material, aiTextureType p_Type, std::string p_TypeName);

public:
	/*!
		\brief Constructor.
		\param p_FilePath the file path to the model.
	*/
	Model(std::string p_FilePath);
	/*!
		\brief Constructor.
		\param p_FilePath the file path to the model.
		\param p_ModelLoaded sets this to true if the model loaded successfully, false otherwise.
	*/
	Model(const std::string &p_FilePath, bool &p_ModelLoaded);

	/*!
		\brief Renders the model.
		\param p_ShaderProgram the shader program ID, being used, to render the model.
	*/
	void Render(const unsigned int p_ShaderProgram);

	/*!
		\brief Loads textures from a file/folder.
		\param p_FilePath the file where the texture is.
		\param p_Directory the directory where the textures are located.
		\param p_Gamma if the texture needs to be gamma corrected
		\return Returns the texture ID.
	*/
	static unsigned int TextureFromFile(const char *p_FilePath, const std::string &p_Directory, bool p_Gamma = false);


	/*!
	\brief Loads textures from a file/folder.
	\param p_FilePath the file where the texture is.
	\param p_Directory the directory where the textures are located.
	\param p_Gamma if the texture needs to be gamma corrected
	\return Returns the texture ID.
	*/
	static unsigned int TextureCubeFromFile(std::vector<const char *> p_FilePath, const std::string &p_Directory, bool p_Gamma = false);
};