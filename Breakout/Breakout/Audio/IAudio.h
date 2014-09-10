#ifndef IAUDIO_H
#define IAUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

#include "../Storage/FileManager.h"


class IAudio
{
private:

protected:

public:

	virtual bool Initialize() = 0;

	/* 
	Play an audio chunk.
	If '_loop' is greater than zero, loop the sound that many times.
	If '_loop' is -1, loop inifinitely (~65000 times).
	Returns true on success.
	*/
	virtual bool PlayMusic(const char* _fileName, int _loop) = 0;

	/* 
	Play an audio chunk on a specific channel.
	If the specified channel is -1, play on the first free channel.
	If '_loop' is greater than zero, loop the sound that many times.
	If '_loop' is -1, loop inifinitely (~65000 times).
	Returns true on success.
	*/
	virtual bool PlaySoundEffect(const char* _fileName, int _loop) = 0;
};

#endif