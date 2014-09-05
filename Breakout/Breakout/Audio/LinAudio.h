#ifndef LINAUDIO_H
#define LINAUDIO_H

#include "IAudio.h"

class LinAudio : public IAudio
{

private:

public:

	LinAudio();
	~LinAudio();

	bool Initialize();

	bool PlayMusic(const char* _fileName, int _loop);
	bool PlaySoundEffect(const char* _fileName, int _loop);

	bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music);
	bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect);


};


#endif