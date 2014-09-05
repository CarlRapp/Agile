#ifndef IAUDIO_H
#define IAUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <map>


class IAudio
{
private:

protected:

	std::map<std::string, Mix_Chunk*> m_soundEffects;
	std::map<std::string, Mix_Music*> m_music;

public:

	virtual bool Initialize() = 0;

	virtual bool PlayAudio(const char* _fileName) = 0;
	virtual bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music) = 0;
	virtual bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect) = 0;
};

#endif