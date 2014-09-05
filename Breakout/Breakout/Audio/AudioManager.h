#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "WinAudio.h"
#include "LinAudio.h"

class AudioManager
{

private:

	IAudio* m_Audio;

public:

	AudioManager();
	~AudioManager();

	bool Initialize();

	bool PlayAudio(const char* _fileName);
	bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music);
	bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect);

};


#endif