#include "SystemComponents.h"


SystemComponents::SystemComponents(void)
{
}

SystemComponents::~SystemComponents(void)
{
}

bool SystemComponents::Initialize(void)
{
	m_AudioManager = new AudioManager();

	if (!m_AudioManager->Initialize())
		return false;
}

AudioManager* SystemComponents::getAudioManager(void)
{
	return m_AudioManager;
}