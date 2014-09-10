#include "AudioManager.h"

AudioManager::AudioManager()
:m_initialized(false)
{
}

AudioManager::~AudioManager()
{
}

bool AudioManager::Initialize()
{
	if (m_initialized)
		return true;

#if defined(_WIN32) || defined(_WIN64)
	m_audio = new WinAudio();
#elif defined(unix) || defined(__unix__) || defined(__unix)
	m_Audio = new LinAudio();
#endif
	
	if (!m_audio->Initialize())
		return false;

	m_initialized = true;

	return true;
}

bool AudioManager::PlayMusic(const char* _fileName, int _loop)
{

	if (!m_audio->PlayMusic(_fileName,_loop))
		return false;

	return true;
}

bool AudioManager::PlaySoundEffect(const char* _fileName, int _loop)
{

	if (!m_audio->PlaySoundEffect(_fileName, _loop))
		return false;

	return true;
}