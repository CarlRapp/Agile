#include "AudioManager.h"
#ifdef WINDOWS
#include "WinAudio.h"
#else
#include "LinAudio.h"
#endif

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


#ifdef WINDOWS
	m_audio = new WinAudio();
#else
	m_audio = new LinAudio();
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