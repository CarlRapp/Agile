#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"

#include "ComponentsSystem/Entity/Entity.h"
#include "ComponentsSystem/Systems/MovementSystem.h"
#include "ComponentsSystem/Components/PositionComponent.h"
#include "ComponentsSystem/Components/VelocityComponent.h"

#ifdef WINDOWS
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;
InputManager* m_InputManager;
FileManager* m_FileManager;

#define MAX_ENTITY_COUNT 500
Entity** m_entities;
MovementSystem m_movementSystem;
Entity* CreateEntity(void)
{

	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		if (m_entities[i]->GetState() == Entity::DEAD)
			return m_entities[i];
	}

	return 0;
}

int main(int argc, char** argv)
{

    time_t startTime = time(0);
	double secondsElapsed;
	DisplayMode displayMode = DisplayMode::BorderlessWindow;


    m_GraphicsManager = GraphicsManager::GetInstance();
	if (!m_GraphicsManager->InitWindow(100, 100, 1000, 800, displayMode))
		return false;
	if (!m_GraphicsManager->Init3D(displayMode))
		return false;
        
	m_AudioManager = &AudioManager::GetInstance();
	if (!m_AudioManager->Initialize())
		return false;

	m_FileManager = FileManager::GetInstance();
	//m_FileManager->LoadSoundEffect("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Kettle-Drum-1.wav");
	m_FileManager->LoadMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clarinet-C5.wav");
	m_AudioManager->PlayMusic("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Yamaha-SY-35-Clarinet-C5.wav", -1);

	Mouse* mouse = m_InputManager->GetInstance()->getInputDevices()->GetMouse();

	

	

	//Mix_Chunk* chunk = FileManager::GetInstance().LoadSoundEffect("Kettle-Drum-1.wav");


	while (difftime(time(0), startTime) < 5)
	{
		m_InputManager->GetInstance()->Update();

		if(mouse->GetButtonState(VK_LBUTTON ) == InputState::Pressed)
			m_AudioManager->PlaySoundEffect("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Kettle-Drum-1.wav", 0);
		if(mouse->GetButtonState(VK_RBUTTON) == InputState::Down)
			m_AudioManager->PlaySoundEffect("C:/Users/Kvagga/Desktop/Agile/Data/Audio/Electric-Bass-Low-C-Staccato.wav", 0);

		m_GraphicsManager->Render();
	}

	return 0;
}
//
//int LinuxInit()
//{
//    
//    return 0;
//}
//
//int WindowsInit()
//{
//    
//    return 0;
//}
