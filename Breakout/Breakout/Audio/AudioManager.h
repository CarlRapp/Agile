#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "IAudio.h"

class AudioManager
{

private:

	AudioManager();
	

	static AudioManager* m_imInstance;

	IAudio* m_audio;

public:

	~AudioManager();
	static AudioManager* GetInstance();

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
        
        void SetMusicVolume(int _volume);
        
        void StopMusic();

};


#endif