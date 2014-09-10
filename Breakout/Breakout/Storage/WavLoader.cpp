#include "WavLoader.h"

Mix_Chunk* WavLoader::LoadWavFile(std::string filePath)
{
	Mix_Chunk* soundEffect = Mix_LoadWAV(filePath.c_str());

	return soundEffect;
}