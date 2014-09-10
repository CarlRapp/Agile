#include "Graphics/GraphicsManager.h"
#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include <SDL.h>

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;
InputManager* m_InputManager;


int main(int argc, char** argv)
{

	m_GraphicsManager = GraphicsManager::GetInstance();
	m_GraphicsManager->InitWindow(100, 100, 1000, 800, DisplayMode::BorderlessWindow);

	Mouse* mouse = m_InputManager->GetInstance()->getInputDevices()->GetMouse();


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
	

	while (true)
	{
		m_InputManager->GetInstance()->Update();

		if (mouse->GetButtonState(VK_LBUTTON) == InputState::Pressed)
			m_AudioManager->PlaySoundEffect("Effect1");

		SDL_Delay(100);

	}






	system("pause");

	return 0;
}