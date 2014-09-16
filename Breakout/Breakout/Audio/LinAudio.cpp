#include "LinAudio.h"

LinAudio::LinAudio() 
: IAudio()
{
}

LinAudio::~LinAudio()
{

	Mix_Quit();
	SDL_Quit();

}

bool LinAudio::Initialize()
{

	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		return false;
	}



	return true;
}

bool LinAudio::PlayMusic(const char* _fileName, int _loop)
{
	Mix_Music* music = FileManager::GetInstance().LoadMusic(_fileName);
	if (!music)
	{
		//Log(MSG_ERROR, "SDL_Audio, Music not found:", _fileName);
		return false;
	}

	// Play the music track
	if (Mix_PlayMusic(music, _loop) == -1)
	{
		return false;
	}

	return true;
}


bool LinAudio::PlaySoundEffect(const char* _fileName, int _loop)
{
	Mix_Chunk* soundEffect = FileManager::GetInstance().LoadSoundEffect(_fileName);
	if (!soundEffect)
	{
		return false;
	}

	// Play the sound
	if (Mix_PlayChannel(-1, soundEffect, _loop) == -1)
	{
		return false;
	}

	return true;

}