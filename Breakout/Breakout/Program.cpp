#include "Graphics/GraphicsManager.h"
#include "Audio/AudioManager.h"

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;


int main(int argc, char** argv)
{

	m_GraphicsManager = new GraphicsManager();
	m_GraphicsManager->InitWindow(600, 400);


	m_AudioManager = new AudioManager();
	if (!m_AudioManager->Initialize())
		return false;

	if (!m_AudioManager->LoadMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clarinet-C5.wav", 0))
		return false;
	if (!m_AudioManager->LoadMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clarinet-C5.wav", 0))
		return false;
	if (!m_AudioManager->LoadMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clari3254234net-C5.wav", 0))
		return false;

	m_AudioManager->PlayAudio("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clari3254234net-C5.wav");

	system("pause");

	return 0;
}