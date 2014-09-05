#include "WinAudio.h"

WinAudio::WinAudio() 
: IAudio()
{
}

WinAudio::~WinAudio()
{
}

bool WinAudio::Initialize()
{

	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("[Windows] SDL_Audio Error: %s\n", SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("[Windows] SDL_Audio Error: %s\n", SDL_GetError());
		return false;
	}



	return true;
}

bool WinAudio::LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music)
{
	std::map<std::string, Mix_Music*>::iterator it = m_music.find(_fileName);
	if (it != m_music.end())
	{
		// Log warning, music already exist
		return true;
	}

	Mix_Music* music = NULL;

	music = Mix_LoadMUS(_filePath);
	if (music == NULL)
	{
		printf("[Windows] SDL_Audio Error: %s\n", Mix_GetError());
		return false;
	}

	_music = music;

	// Temp for testing
	m_music.insert(std::pair<std::string, Mix_Music*>(_fileName, music));

	return true;
}

bool WinAudio::LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect)
{
	std::map<std::string, Mix_Chunk*>::iterator it = m_soundEffects.find(_fileName);
	if (it != m_soundEffects.end())
	{
		// Log warning, soundEffect already exist
		return true;
	}

	Mix_Chunk* soundEffect = NULL;

	soundEffect = Mix_LoadWAV(_filePath);
	if (soundEffect == NULL)
	{
		printf("[Windows] SDL_Audio Error: %s\n", Mix_GetError());
		return false;
	}

	_soundEffect = soundEffect;

	// Temp for testing
	m_soundEffects.insert(std::pair<std::string, Mix_Chunk*>(_fileName, soundEffect));

	return true;
}

bool WinAudio::PlayAudio(const char* _fileName)
{
	/*
	Access AudioData
	Find if _fileName exist
	Get the Mix_Music* / Mix_Chunk*
	Play the sound
	(Probably split this into two (PlayMusic & PlaySoundEffect)
	*/



	std::map<std::string, Mix_Music*>::iterator musicIter = m_music.find(_fileName);
	if (musicIter != m_music.end())
	{
		// Log warning, music already exist
		return true;
	}





	std::map<std::string, Mix_Chunk*>::iterator soundEffectIter = m_soundEffects.find(_fileName);

	// SoundEffect found
	if (soundEffectIter != m_soundEffects.end())
	{
		
		return true;
	}






	return true;
}