#ifndef MUSICLOADER_H
#define MUSICLOADER_H

#include <SDL.h>
#include <SDL_mixer.h>
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
