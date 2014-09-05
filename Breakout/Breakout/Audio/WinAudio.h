#ifndef WINAUDIO_H
#define WINAUDIO_H

#include "IAudio.h"

class WinAudio : public IAudio
{

private:

public:
	WinAudio();
	~WinAudio();

	bool Initialize();

	bool PlayAudio(const char* _fileName);
	bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music);
	bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect);

};


#endif