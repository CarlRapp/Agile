#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "WinAudio.h"
#include "LinAudio.h"

class AudioManager
{

private:

	AudioManager();

	static AudioManager* m_audioManager;
	bool m_initialized;

	IAudio* m_audio;

public:

	// Returns an AudioManager instance.
	static AudioManager* getInstance();
	
	~AudioManager();

	// Initialize the AudioManager, only needed to do once.
	// Returns true on success.
	bool Initialize();

	/*
	Play an audio chunk.
	If '_loop' is greater than zero, loop the sound that many times.
	If '_loop' is -1, loop inifinitely (~65000 times).
	Returns true on success.
	*/
	bool PlayMusic(const char* _fileName, int _loop = 0);

	/*
	Play an audio chunk on a specific channel.
	If the specified channel is -1, play on the first free channel.
	If '_loop' is greater than zero, loop the sound that many times.
	If '_loop' is -1, loop inifinitely (~65000 times).
	Returns true on success.
	*/
	bool PlaySoundEffect(const char* _fileName, int _loop = 0);

	/*
	Load a wave file or a music (.mod .s3m .it .xm) file.
	_filePath defines the location of the file.
	_fileName is the desired name for said file.
	_music will return a pointer to the loaded file.
	Returns true on success.
	*/
	bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music);

	/*
	Load a wave file or a music (.mod .s3m .it .xm) file.
	_filePath defines the location of the file.
	_fileName is the desired name for said file.
	_music will return a pointer to the loaded file.
	Returns true on success.
	*/
	bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect);

};


#endif