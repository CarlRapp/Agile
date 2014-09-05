#include "Graphics/GraphicsManager.h"
#include "Audio/AudioManager.h"

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;


int main(int argc, char** argv)
{

	m_GraphicsManager = new GraphicsManager();
	m_GraphicsManager->InitWindow(600, 400);


	m_AudioManager = AudioManager::getInstance();
	if (!m_AudioManager->Initialize())
		return false;

	if (!m_AudioManager->LoadMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clarinet-C5.wav", "Sound1", 0))
	{
		system("pause");
		return false;
	}

	if (!m_AudioManager->LoadMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Kettle-Drum-1.wav", "Sound2", 0))
	{
		system("pause");
		return false;
	}

	if (!m_AudioManager->LoadSoundEffect("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Electric-Bass-Low-C-Staccato.wav", "Effect1", 0))
	{
		system("pause");
		return false;
	}

	m_AudioManager->PlayMusic("Sound1",-1);
	m_AudioManager->PlaySoundEffect("Effect1",4);
	Sleep(3000);
	m_AudioManager->PlayMusic("Sound2", -1);

	if (!m_AudioManager->LoadMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clarinet-C5.wav", "Sound1", 0))
	{
		system("pause");
		return false;
	}

	if (!m_AudioManager->LoadMusic("C:/Users/Kvagga/Drthrtv", "ergerg", 0))
	{
		system("pause");
		return false;
	}

	system("pause");

	return 0;
}