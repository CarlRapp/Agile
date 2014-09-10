#include "Graphics/GraphicsManager.h"
#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"
#include <SDL.h>

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;
InputManager* m_InputManager;


int main(int argc, char** argv)
{

	m_GraphicsManager = GraphicsManager::GetInstance();
	m_GraphicsManager->InitWindow(600, 400);

	Mouse* mouse = m_InputManager->GetInstance()->getInputDevices()->GetMouse();

	m_AudioManager = &AudioManager::GetInstance();

	if (!m_AudioManager->Initialize())
		return false;

	m_AudioManager->PlaySoundEffect("Kettle-Drum-1.wav", -1);

	//Mix_Chunk* chunk = FileManager::GetInstance().LoadSoundEffect("Kettle-Drum-1.wav");
	
	

	system("pause");

	return 0;
}