#include "AudioManager.h"
#ifdef WINDOWS
#include "WinAudio.h"
#else
#include "LinAudio.h"
#endif
#include "../stdafx.h"

AudioManager::AudioManager()
{
#ifdef WINDOWS
	m_audio = new WinAudio();
#else
	m_audio = new LinAudio();
#endif

	m_audio->Initialize();
}

AudioManager::~AudioManager()
{
}



AudioManager* AudioManager::m_imInstance = 0;
AudioManager* AudioManager::GetInstance()
{
	if (m_imInstance)
		return m_imInstance;

	m_imInstance = new AudioManager();

	return m_imInstance;
}

bool AudioManager::PlayMusic(const char* _fileName, int _loop)
{

	if (!m_audio->PlayMusic(GetFile(_fileName,AUDIO_ROOT).c_str(),_loop))
		return false;

	return true;
}

bool AudioManager::PlaySoundEffect(const char* _fileName, int _loop)
{

	if (!m_audio->PlaySoundEffect(GetFile(_fileName,AUDIO_ROOT).c_str(), _loop))
		return false;

	return true;
}