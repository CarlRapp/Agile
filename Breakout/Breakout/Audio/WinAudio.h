#ifndef WINAUDIO_H
#define WINAUDIO_H

#include "IAudio.h"
//#include <Windows.h>


enum MESSAGE_TYPE
{
	MSG_NORMAL = 0,
	MSG_WARNING,
	MSG_ERROR
};

class WinAudio : public IAudio
{

private:

public:
	WinAudio();
	~WinAudio();

	bool Initialize();

	bool PlayMusic(const char* _fileName, int _loop);
	bool PlaySoundEffect(const char* _fileName, int _loop);

	bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music);
	bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect);
        
        void SetVolume(int _volume);
        
        void StopMusic();
};


#endif