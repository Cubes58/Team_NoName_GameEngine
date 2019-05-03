#include "DefaultGame.h"

#include <string>
#include <cctype>
#include <memory>
#include <iostream>

#include "IEngineCore.h"
#include "RenderEngine.h"
#include "FileSystemHelper.h"
#include "InputHandler.h"
#include "Scene.h"
#include "PlayerCharacter.h"

static const std::string s_DEFAULT_SCENE = "resources/levels/default0.json";

DefaultGame::DefaultGame() {
	
	ImGui::CreateContext();
	imguiInit();
	LoadScenes("resources/scenes/");
}

DefaultGame::~DefaultGame() {
	
}

void DefaultGame::LoadScenes(const std::string &p_FolderLocation) {
	std::vector<FileInformation> sceneLevels = FileSystemHelper::GetFilesInFolder(p_FolderLocation);
	FileSystemHelper::RetainRemoveFilesWithExtensions(sceneLevels, { ".json" });	// Only keep the .json files.

	std::vector<std::string> informationToRemove = { "scene", "Scene", "scenes", "Scenes", "level", "Level", "levels", "Levels", "default", "Default" };
	for (FileInformation &sceneLevel : sceneLevels) {
		for (const std::string &infoToRemove : informationToRemove) {
			std::string::size_type locationFound = sceneLevel.m_Name.find(infoToRemove);

			if (locationFound != std::string::npos) {
				sceneLevel.m_Name.erase(locationFound, infoToRemove.length());
				continue;
			}
		}

		if (sceneLevel.m_Name.size() == 0 || sceneLevel.m_Location.size() == 0)
			continue;

		// Ensure the first character in the string is a digit. Otherwise stoi throws and error, and cannot convert the string to an int.
		if (std::isdigit(static_cast<int>(sceneLevel.m_Name.at(0))))
			m_Scenes.emplace(std::stoi(sceneLevel.m_Name), sceneLevel.m_Location);
		else {
			std::cerr << "WARNING::GAME: Invalid naming of a scene: " << sceneLevel.m_Name << std::endl;

			std::string name = sceneLevel.m_Name;
			name.at(0) = '1';
			m_Scenes.emplace(std::stoi(name), sceneLevel.m_Location);
		}
	}

	// If there aren't any scenes:
	if (m_Scenes.size() <= 0) {
		std::cerr << "ERROR::GAME: No scenes were found, to load." << std::endl;
		m_IsRunning = false;
	}
}


void DefaultGame::SetScene(unsigned int p_SceneNumber) {
	m_SwitchingScene = true;
	auto iter = m_Scenes.find(p_SceneNumber);
	if (iter != m_Scenes.end()) {
		m_CurrentSceneNumber = p_SceneNumber;
		m_CurrentScene = std::make_shared<Scene>(iter->second, this->shared_from_this());
	}
	else {
		m_CurrentScene = std::make_shared<Scene>(s_DEFAULT_SCENE, this->shared_from_this());
		std::cerr << "ERROR::GAME: COULDNT FIND SCENE NUMBER: " << p_SceneNumber <<
			"\nNOT FOUND, ELEMENT LOCATION: " << iter->first << "\tSCENE LOCATION: " << iter->second << std::endl;
	}

	m_EngineInterface->m_PhysicsEngine->GiveObjects(m_CurrentScene->GetObjects());

	m_InputHandler->SetObjectsRequiringInput(m_CurrentScene->GetObjectsRequiringInput());
	m_InputHandler->LoadKeyBinds(m_InputHandler->GetKeyBindFile());
}

void DefaultGame::Update(float p_DeltaTime) {
	m_SwitchingScene = false;

	m_CurrentScene->Update(p_DeltaTime);
}

void DefaultGame::Render() {
	if (!m_SwitchingScene) {
		m_CurrentScene->Render(m_EngineInterface);	
	}

	RenderEngineInstance.RenderText("Scene number: " + std::to_string(m_CurrentSceneNumber), 0.005f, 0.955f, 0.45f, glm::vec3(0.0f, 0.5f, 0.5f));

	imguiRender();
}

void DefaultGame::imguiInit() {

}

void DefaultGame::imguiRender() {

	//m_CurrentScene->ImGuiRender();
}

void DefaultGame::imguiShutdown() {

}

unsigned int DefaultGame::GetNextScene() {
	if(m_Scenes.size() == 1) {
		CloseGame();
		return m_Scenes.begin()->first;
	}

	auto iter = m_Scenes.find(m_CurrentSceneNumber);
	if (iter != m_Scenes.end()) {
		++iter;
		if (iter != m_Scenes.end())
			return iter->first;
		else {
			CloseGame();
			// Loop around, to the start.
			return m_Scenes.begin()->first;
		}
	}

	return m_CurrentSceneNumber;
}

unsigned int DefaultGame::GetPreviousScene() {
	if(m_Scenes.size() == 1) {
		CloseGame();
		return m_Scenes.begin()->first;
	}

	auto iter = m_Scenes.find(m_CurrentSceneNumber);
	if (iter != m_Scenes.end()) {
		// If the iter isn't the first element in m_Scene, we can go backwards.
		if (iter != m_Scenes.begin()) {
			--iter;	// Go backwards, to the previous element.
			if (iter != m_Scenes.end())
				return iter->first;
		}
		else {
			CloseGame();
			auto loopToEnd = m_Scenes.end();
			--loopToEnd;
			return loopToEnd->first;
		}
	}

	return m_CurrentSceneNumber;
}