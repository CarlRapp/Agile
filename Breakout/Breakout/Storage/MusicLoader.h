#ifndef MUSICLOADER_H
#define MUSICLOADER_H

#include "../stdafx.h"

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

class MusicLoader
{
private:
	MusicLoader();
	~MusicLoader();
public:
	static Mix_Music* LoadMusicFile(std::string filePath);
};
#endif