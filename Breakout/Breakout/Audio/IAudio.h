#ifndef IAUDIO_H
#define IAUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
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

	/* 
	Load a wave file or a music (.mod .s3m .it .xm) file.
	_filePath defines the location of the file.
	_fileName is the desired name for said file.
	_music will return a pointer to the loaded file.
	Returns true on success.
	*/
	virtual bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music) = 0;

	/*
	Load a wave file or a music (.mod .s3m .it .xm) file.
	_filePath defines the location of the file.
	_fileName is the desired name for said file.
	_music will return a pointer to the loaded file.
	Returns true on success.
	*/
	virtual bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect) = 0;
};

#endif