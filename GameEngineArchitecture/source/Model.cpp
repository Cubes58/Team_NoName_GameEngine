#include "Model.h"
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <iostream>

Model::Model(std::string p_FilePath) {
	LoadModel(p_FilePath);
}

Model::Model(const std::string &p_FilePath, bool &p_ModelLoaded) {
	p_ModelLoaded = LoadModel(p_FilePath);
}

void Model::Render(const unsigned int p_ShaderProgram) {
	for (auto mesh : m_Meshes) {
		mesh.Render(p_ShaderProgram);
	}
}

bool Model::LoadModel(std::string p_FilePath) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(p_FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return false;
	}
	m_Directory = p_FilePath.substr(0, p_FilePath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

	return true;
}

void Model::ProcessNode(aiNode *p_Node, const aiScene *p_Scene) {
	// Get the meshes of the node and add them to our vector.
	for (int i = 0; i < p_Node->mNumMeshes; i++) {
		int sceneMeshIndex = p_Node->mMeshes[i];
		aiMesh* mesh = p_Scene->mMeshes[sceneMeshIndex];
		m_Meshes.push_back(ProcessMesh(mesh, p_Scene));
	}
	// Recursively process the nodes of any children.
	for (int i = 0; i < p_Node->mNumChildren; i++) {
		ProcessNode(p_Node->mChildren[i], p_Scene);
	}
}

Mesh Model::ProcessMesh(aiMesh *p_Mesh, const aiScene *p_Scene) {
	// Data to fill.
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Create enough space for the vertices and indices.
	vertices.resize(p_Mesh->mNumVertices);
	indices.resize(p_Mesh->mNumFaces * 3); // Imported as triangles.

										 // For each vertex of the mesh copy the data to out own mesh format.
	for (unsigned int i = 0; i < p_Mesh->mNumVertices; i++) {
		// All meshes should have vertices and normals.
		vertices[i].m_Position = glm::vec3(p_Mesh->mVertices[i].x, p_Mesh->mVertices[i].y, p_Mesh->mVertices[i].z);
		vertices[i].m_Normal = glm::vec3(p_Mesh->mNormals[i].x, p_Mesh->mNormals[i].y, p_Mesh->mNormals[i].z);

		// Check if the mesh has texture coordinates.
		if (p_Mesh->mTextureCoords[0]) {
			vertices[i].m_TextureCoordinates = glm::vec2(p_Mesh->mTextureCoords[0][i].x, p_Mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices[i].m_TextureCoordinates = glm::vec2(0.0f, 0.0f);
		}
	}

	// Save all the vertex indices in the indices vector.
	for (int i = 0; i < p_Mesh->mNumFaces; i++) {
		// Retrieve all indices of the face and store them in the indices vector.
		for (int j = 0; j < p_Mesh->mFaces[i].mNumIndices; j++)
			indices[3 * i + j] = p_Mesh->mFaces[i].mIndices[j];
	}

	// Get material textures (if there are any).
	if (p_Mesh->mMaterialIndex >= 0) {
		aiMaterial* material = p_Scene->mMaterials[p_Mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");

		// Put all the textures together in a single vector.
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	// Return the mesh data.
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *p_Material, aiTextureType p_Type, std::string p_TypeName) {
	std::vector<Texture> textures;
	for (int i = 0; i < p_Material->GetTextureCount(p_Type); i++) {
		aiString str;
		p_Material->GetTexture(p_Type, i, &str);
		bool b_loadedTexture = false;
		for (auto texture : m_Textures) {
			if (std::strcmp(texture.p_FilePath.C_Str(), str.C_Str()) == 0) {
				textures.push_back(texture);
				b_loadedTexture = true;
				break;
			}
		}
		if (!b_loadedTexture) {
			// Setup a new texture.
			Texture texture;
			texture.m_ID = TextureFromFile(str.C_Str(), m_Directory);
			texture.m_Type = p_TypeName;
			texture.p_FilePath = str;
			textures.push_back(texture);
			m_Textures.push_back(texture); // Add to loaded textures.
		}
	}
	return textures;
}

// Static function to load a texture using lightweight stb_image library.
unsigned int Model::TextureFromFile(const char *p_FilePath, const std::string &p_Directory, bool p_Gamma) {
	std::string filename = std::string(p_FilePath);
	filename = p_Directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (textureData) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(textureData);
	}
	else {
		std::cout << "Texture failed to load from: " << p_FilePath << std::endl;
		stbi_image_free(textureData);
	}

	return textureID;
}

unsigned int Model::TextureCubeFromFile(std::vector<const char*> p_FilePath, const std::string & p_Directory, bool p_Gamma)
{
	unsigned int l_textureID;
	glGenTextures(1, &l_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, l_textureID);

	int l_ImageWidth, l_ImageHeight, l_NrComponents;
	unsigned char* l_TextureData;

	for (GLuint i = 0; i < p_FilePath.size(); i++)
	{
		l_TextureData = stbi_load(p_FilePath[i], &l_ImageWidth, &l_ImageHeight, &l_NrComponents, 0);

		if (l_TextureData)
		{
			GLenum format;
			if (l_NrComponents == 1)
				format = GL_RED;
			else if (l_NrComponents == 3)
				format = GL_RGB;
			else if (l_NrComponents == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, l_ImageWidth, l_ImageHeight, 0, format, GL_UNSIGNED_BYTE, l_TextureData);
		}
		else
		{
			std::cout << "Texture failed to load from: " << p_FilePath[i] << std::endl;
		}
		stbi_image_free(l_TextureData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return l_textureID;
}
