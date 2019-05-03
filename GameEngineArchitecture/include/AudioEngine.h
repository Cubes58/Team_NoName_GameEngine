#pragma once

#include <map>
#include <string>
#include <SFML/Audio.hpp>

#define AudioManagerInstance AudioManager::Instance()

class AudioManager {
private:
	struct Audio {
		sf::SoundBuffer m_SoundBuffer;
		sf::Sound m_Sound;

		Audio() = default;
		Audio(sf::SoundBuffer p_SoundBuffer) : m_SoundBuffer(p_SoundBuffer) {
		}
	};
	std::map<std::string, Audio> m_SoundEffects;
	sf::Music m_BackgroundMusic;

	AudioManager();
	~AudioManager() = default;

public:
	static AudioManager &Instance();

	bool OpenMusicFile(const std::string &p_PathToAudioFile, bool p_Play = true);
	bool LoadSoundEffect(const std::string &p_Name, const std::string &p_PathToAudioFile);
	bool LoadSoundEffects(const std::string &p_PathToSoundEffectsFolder);
	bool PlaySoundEffect(const std::string &p_Name);

	const sf::SoundBuffer *GetSoundEffect(const std::string &p_Name) const;
	sf::Music &GetBackgroundMusic();

	// Delete the copy and assignment operators.
	AudioManager(AudioManager const&) = delete;
	AudioManager& operator=(AudioManager const&) = delete;	
};