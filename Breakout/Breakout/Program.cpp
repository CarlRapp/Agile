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

	DisplayMode displayMode = DisplayMode::BorderlessWindow;


        double secondsElapsed;

        m_GraphicsManager = GraphicsManager::GetInstance();
        
	m_GraphicsManager->InitWindow(100, 100, 1000, 800, displayMode);

	m_GraphicsManager->Init3D(displayMode);
        
	//Mouse* mouse = m_InputManager->GetInstance()->getInputDevices()->GetMouse();

	m_AudioManager = &AudioManager::GetInstance();

	if (!m_AudioManager->Initialize())
		return false;

	//m_AudioManager->PlaySoundEffect("Kettle-Drum-1.wav", -1);

	//Mix_Chunk* chunk = FileManager::GetInstance().LoadSoundEffect("Kettle-Drum-1.wav");




	//Init
	m_entities = new Entity*[MAX_ENTITY_COUNT];

	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
		m_entities[i] = new Entity(i);
	//Init done

	//Create new Entity
	auto player = CreateEntity();
	//Add Components
	player->AddComponent<VelocityComponent>();
	player->AddComponent<PositionComponent>();
	//Player done
	player->SetState(Entity::ACTIVATED);

	// UPDATE CALL - START


	for (int i = 0; i < 2; ++i)
	{


		m_movementSystem.Clear();

		for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
		{
			if (m_entities[i]->GetState() == Entity::ACTIVATED)
			{
				if (m_movementSystem.GetComponentFilter().DoesFilterPass(m_entities[i]->GetComponents()))
					m_movementSystem.Add(m_entities[i]);
			}
		}

		m_movementSystem.Update(0.5f);

	}

	// UPDATE CALL - END






        while(difftime(time(0),startTime)<5)
            m_GraphicsManager->Render();

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
