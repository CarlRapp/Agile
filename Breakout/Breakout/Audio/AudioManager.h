#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "IAudio.h"

class AudioManager
{

private:

	AudioManager();
	~AudioManager();

	bool m_initialized;

	IAudio* m_audio;

public:

	// Returns an AudioManager instance.
	static AudioManager& GetInstance()
	{
		static AudioManager* instance = new AudioManager();
		return *instance;
	}
	
	

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

};


#endif