#include "LinAudio.h"

LinAudio::LinAudio()
: IAudio()
{
}

LinAudio::~LinAudio()
{
	for (std::map<std::string, Mix_Music*>::iterator iter = m_music.begin(); iter != m_music.end(); ++iter)
	{
		Mix_FreeMusic(iter->second);
		iter->second = NULL;
	}

	for (std::map<std::string, Mix_Chunk*>::iterator iter = m_soundEffects.begin(); iter != m_soundEffects.end(); ++iter)
	{
		Mix_FreeChunk(iter->second);
		iter->second = NULL;
	}

	Mix_Quit();
	SDL_Quit();

}

bool LinAudio::Initialize()
{
	return true;
}

bool LinAudio::LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music)
{
	return true;
}

bool LinAudio::LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect)
{
	return true;
}

bool LinAudio::PlayMusic(const char* _fileName, int _loop)
{
	return true;
}

bool LinAudio::PlaySoundEffect(const char* _fileName, int _loop)
{
	return true;
}