#include "AudioManager.h"

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{
}

bool AudioManager::Initialize()
{

#if defined(_WIN32) || defined(_WIN64)
	m_Audio = new WinAudio();
#elif defined(unix) || defined(__unix__) || defined(__unix)
	m_Audio = new LinAudio();
#endif
	
	if (!m_Audio->Initialize())
		return false;

	return true;
}

bool AudioManager::LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music)
{
	if (!m_Audio->LoadMusic(_filePath, _fileName, _music))
		return false;

	return true;
}

bool AudioManager::LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect)
{

	if (!m_Audio->LoadSoundEffect(_filePath, _fileName, _soundEffect))
		return false;

	return true;
}

bool AudioManager::PlayAudio(const char* _fileName)
{

	if (!m_Audio->PlayAudio(_fileName))
		return false;

	return true;
}