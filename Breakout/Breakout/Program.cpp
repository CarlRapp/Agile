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

	m_AudioManager = &AudioManager::GetInstance();

	if (!m_AudioManager->Initialize())
		return false;
        
        /*  INPUT MANAGER   */
        m_InputManager = InputManager::GetInstance();
        Keyboard* kb = m_InputManager->getInputDevices()->GetKeyboard();
        Mouse* mus = m_InputManager->getInputDevices()->GetMouse();

	//m_AudioManager->PlaySoundEffect("Kettle-Drum-1.wav", 0);
        //m_AudioManager->PlayMusic("Electric-Bass-Low-C-Staccato.wav",-1);
	//Mix_Chunk* chunk = FileManager::GetInstance().LoadSoundEffect("Kettle-Drum-1.wav");

        //m_AudioManager->PlayMusic(GetFile("Kettle-Drum-1.wav", AUDIO_ROOT).c_str(), -1);
        int a = 0, b = 0, c = 0;
        while(difftime(time(0),startTime)<10)
        {
            m_InputManager->Update();
            if(kb->GetKeyState('w') == InputState::Pressed)
            {
                m_GraphicsManager->GetICamera()->Move(Vector3(0,1.0f,0));
            }
            if(kb->GetKeyState('s') == InputState::Pressed)
            {
                m_GraphicsManager->GetICamera()->Move(Vector3(0,-1.0f,0));
            }
            if(kb->GetKeyState('a') == InputState::Pressed)
            {
                m_GraphicsManager->GetICamera()->Move(Vector3(-1.0f,0,0));
            }
            if(kb->GetKeyState('d') == InputState::Pressed)
            {
                m_GraphicsManager->GetICamera()->Move(Vector3(1.0f,0,0));
            }
            
            //if(a==10)
            //    break;
               
            
            //if(mus->GetButtonState(MouseButton::RightMB)== InputState::Pressed)
               //m_AudioManager->PlaySoundEffect(GetFile("Electric-Bass-Low-C-Staccato.wav", AUDIO_ROOT).c_str(), 0);

            if(mus->GetButtonState(MouseButton::MiddleMB) == InputState::Down)
            {
                printf("X: %d\nY: %d\n", mus->getX(), mus->getY());
                printf("dX: %d\ndY: %d\n", mus->getdX(), mus->getdY());
            }
            
            m_GraphicsManager->Render();
        }


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
//
//	while (difftime(time(0), startTime) < 5)
//	{
//		m_GraphicsManager->Update(0.001f);
//		m_GraphicsManager->Render();
//	}
            

		delete m_GraphicsManager;
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
