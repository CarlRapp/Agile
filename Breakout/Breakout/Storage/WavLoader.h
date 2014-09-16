#ifndef WAVLOADER_H
#define WAVLOADER_H

#include "../stdafx.h"
#include <unordered_map>
#ifdef WINDOWS
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif
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
