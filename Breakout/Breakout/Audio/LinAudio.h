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

	bool PlayAudio(const char* _fileName);
	bool LoadAudio(const char* _fileName);


};


#endif