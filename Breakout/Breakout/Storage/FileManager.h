#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_


#include <string>
#include "../stdafx.h"
#include <unordered_map>
#ifdef WINDOWS
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif
#include <typeinfo>


#include "../Storage/ModelData.h"
#include "ModelLoader.h"
#include "WavLoader.h"
#include "MusicLoader.h"

class FileManager
{
private:
	static FileManager* instance;
	FileManager() {};
	~FileManager() {};

	std::unordered_map<std::string, ModelData*> m_modelMap;
	std::unordered_map<std::string, Mix_Chunk*> m_mixChunkMap;
	std::unordered_map<std::string, Mix_Music*> m_mixMusicMap;

public:
	static FileManager& GetInstance()
	{ 
		static FileManager* instance = new FileManager();
		return *instance;
	}

	// Don't include an extension at the end of the file, eg. .obj
	ModelData* LoadModel(std::string filePath);

	// AudioManager needs to be initialized
	Mix_Chunk* LoadSoundEffect(std::string filePath);

	// AudioManager needs to be initialized
	Mix_Music* LoadMusic(std::string filePath);
};

#endif