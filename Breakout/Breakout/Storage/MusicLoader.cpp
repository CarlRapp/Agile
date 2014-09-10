#include "MusicLoader.h"

Mix_Music* MusicLoader::LoadMusicFile(std::string filePath)
{
	Mix_Music* music = Mix_LoadMUS(filePath.c_str());

	return music;
}