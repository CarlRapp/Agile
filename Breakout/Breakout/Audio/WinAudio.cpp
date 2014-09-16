#include "WinAudio.h"

WinAudio::WinAudio() 
: IAudio()
{
}

WinAudio::~WinAudio()
{

	Mix_Quit();
	SDL_Quit();

}

bool WinAudio::Initialize()
{

	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		//Log(MSG_ERROR, "SDL_Audio:", SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		//Log(MSG_ERROR, "SDL_Audio:", SDL_GetError());
		return false;
	}



	return true;
}

bool WinAudio::PlayMusic(const char* _fileName, int _loop)
{
	Mix_Music* music = FileManager::GetInstance().LoadMusic(_fileName);
	if (!music)
	{
		//Log(MSG_ERROR, "SDL_Audio, Music not found:", _fileName);
		return false;
	}

	// Another music already playing, switching
	if (Mix_PlayingMusic() != 0)
	{
	}
		//Log(MSG_WARNING, "SDL_Audio: Already playing track. Changing to new."); // Could possible be removed

	// Play the music track
	if (Mix_PlayMusic(music, _loop) == -1)
	{
		//Log(MSG_ERROR, "SDL_Audio:", Mix_GetError());
		return false;
	}

	return true;
}


bool WinAudio::PlaySoundEffect(const char* _fileName, int _loop)
{
	// Sound not found, return false
	Mix_Chunk* soundEffect = FileManager::GetInstance().LoadSoundEffect(_fileName);
	if (!soundEffect)
	{
		//Log(MSG_ERROR, "SDL_Audio, Sound effect not found:", _fileName);
		return false;
	}

	// Play the sound
	if (Mix_PlayChannel(-1, soundEffect, _loop) == -1)
	{
		//Log(MSG_ERROR, "SDL_Audio:", Mix_GetError());
		return false;
	}

	return true;

}