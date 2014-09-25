#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "ISystem.h"
#include "../../Audio/AudioManager.h"

class AudioSystem : public System<AudioSystem>
{
private:
	AudioManager* m_audioManager;

public:

	AudioSystem(World* _world);
	~AudioSystem();

	void Update(float _dt);
};

#endif