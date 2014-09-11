#ifndef WAVLOADER_H
#define WAVLOADER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>

class WavLoader
{
private:
	WavLoader();
	~WavLoader();
public:
	static Mix_Chunk* LoadWavFile(std::string filePath);
};
#endif
