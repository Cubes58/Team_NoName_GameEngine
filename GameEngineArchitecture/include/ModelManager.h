/**
@file ModelManager.h
@brief A class that manages the models.
*/
#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "Model.h"
#include "FileSystemHelper.h"

#define MODEL_MANAGER ModelManager::GetInstance()	//!< A macro to get the mode manager instance directly.

/*! \class ModelManager
	\brief A class that manages the models.
*/
class ModelManager {
private:
	std::map<std::string, std::shared_ptr<Model>> m_Models;	//!< Stores the models.
	std::vector<std::string> m_UnsuccessfullyLoadedModels;	//!< Stores the file location of any unsuccessfully loaded models.

	/*!
		\brief Default constructor.
	*/
	ModelManager() {
		LoadModelsFromFolder("resources/models");
	}

	~ModelManager() = default;	//!< Default destructor.

public:
	/*!
		\brief Gets the ModelManager instance.
		\return Returns the model manager instance.
	*/
	static ModelManager &GetInstance() {
		static ModelManager instance;

		return instance;
	}

	/*!
		\brief Loads a model in, from a file.
		\param p_FileLocation the location of the file.
		\return Returns true if the model loaded successfully, else it returns false.
	*/
	bool LoadModelFromFile(std::string p_FileLocation) {
		// Load in a model, from a file.
		bool modelLoadedCorrectly = false;

		std::shared_ptr<Model> model = std::make_shared<Model>(p_FileLocation, modelLoadedCorrectly);
		if (modelLoadedCorrectly) {
			FileSystemHelper::RemoveFilePath(p_FileLocation);
			FileSystemHelper::RemoveFileExtension(p_FileLocation);
			m_Models.insert(std::pair<std::string, std::shared_ptr<Model>>(p_FileLocation, model));
		}
		else
			m_UnsuccessfullyLoadedModels.push_back(p_FileLocation);

		return modelLoadedCorrectly;
	}

	/*!
		\brief Loads a model in, from a file.
		\param p_FileLocation the name, location, and extension of the file.
		\return Returns true if the model loaded successfully, else it returns false.
	*/
	bool LoadModelFromFile(const FileInformation &p_FileLocation) {
		// Load in a model, from a file.
		bool modelLoadedCorrectly = false;
		std::shared_ptr<Model> model = std::make_shared<Model>(p_FileLocation.m_Location, modelLoadedCorrectly);
		if (modelLoadedCorrectly) {
			m_Models.insert(std::pair<std::string, std::shared_ptr<Model>>(p_FileLocation.m_Name, model));
		}
		else
			m_UnsuccessfullyLoadedModels.push_back(p_FileLocation.m_Location);

		return modelLoadedCorrectly;
	}

	/*!
		\brief Loads any models from a folder.
		\param p_FolderName the name of the folder.
		\return Returns true if all of the models loaded successfully, else it returns false.
	*/
	bool LoadModelsFromFolder(const std::string &p_FolderName) {
		// Load any models, inside of a certain folder.
		std::vector<FileInformation> modelFiles = FileSystemHelper::GetFilesInFolder(p_FolderName);

		// Assimp doesn't like double back-slashes, for directory locations, so they must manually be changed to a forward slash.
		for (auto &modelFile : modelFiles) {
			while (true) {
				auto position = modelFile.m_Location.find("\\");

				if (position != std::string::npos)
					modelFile.m_Location.replace(position, 1, "/");
				else
					break;
			}
		}

		// Only try to load the model files, with the extensions (.obj, .dae, .fbx...), as Assimp can handle these file formats.
		FileSystemHelper::RetainRemoveFilesWithExtensions(modelFiles, { ".obj", ".dae", ".fbx", ".3ds", ".blend", ".ply", ".stl" });

		bool allLoadedCorrectly = false, loadedModelCorrectly = false;
		for (auto &modelFile : modelFiles) {
			loadedModelCorrectly = LoadModelFromFile(modelFile);

			if (!loadedModelCorrectly)
				allLoadedCorrectly = false;
		}

		return allLoadedCorrectly;
	}

	/*!
		\brief Gets a model pointer, given a model's name.
		\param p_ModelName the name of the model.
		\return Returns the model pointer p_ModelName references if it's found, else it'll try to find the default one, and return that, but if that fails too then a nullptr will be returned.
	*/
	std::shared_ptr<Model> GetModel(const std::string &p_ModelName) {
		std::map<std::string, std::shared_ptr<Model>>::iterator iter = m_Models.find(p_ModelName);
		if (iter != m_Models.end()) {
			return iter->second;
		}

		// If the model requested cannot be found, then return the default model (question mark).
		std::map<std::string, std::shared_ptr<Model>>::iterator iterToDefault = m_Models.find("default");
		if (iterToDefault != m_Models.end()) {
			return iterToDefault->second;
		}

		return std::shared_ptr<Model>(nullptr);
	}

	/*!
		\brief Gets a vector of model name's that failed to be loaded in.
		\return Returns a vector of model name's that failed to be loaded in.
	*/
	std::vector<std::string> GetUnsuccessfullyLoadedModels() {
		return m_UnsuccessfullyLoadedModels;
	}

	// Delete the copy and assignment operators.
	ModelManager(ModelManager const&) = delete; //!< Copy operator.
	ModelManager& operator= (ModelManager const&) = delete; //!< Assignment operator. 
};