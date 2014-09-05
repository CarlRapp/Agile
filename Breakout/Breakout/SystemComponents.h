#ifndef SYSTEMCOMPONENTS_H
#define SYSTEMCOMPONENTS_H

#include "Audio/AudioManager.h"

class SystemComponents
{
private:

	AudioManager* m_AudioManager;

public:

	SystemComponents();
	~SystemComponents();

	bool Initialize(void);

	AudioManager* getAudioManager(void);

};

#endif