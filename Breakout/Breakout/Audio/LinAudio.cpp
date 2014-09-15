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