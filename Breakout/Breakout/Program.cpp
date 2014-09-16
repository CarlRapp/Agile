#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"

#include "ComponentsSystem/Entity/Entity.h"
#include "ComponentsSystem/EntityFactory.h"
#include "ComponentsSystem/Components/ModelComponent.h"

#include "ComponentsSystem/Systems/MovementSystem.h"
#include "ComponentsSystem/Systems/ModelSystem.h"
#include "ComponentsSystem/Systems/ScoreSystem.h"

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
MovementSystem m_movementSystem;
ModelSystem m_modelSystem;
ScoreSystem m_scoreSystem;

#define MAX_ENTITY_COUNT 500
Entity** m_entities;

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
        //int a = 0, b = 0, c = 0;
        //while(difftime(time(0),startTime)<10)
        //{
        //    m_InputManager->Update();
        //    if(kb->GetKeyState('w') == InputState::Pressed)
        //    {
        //        m_GraphicsManager->GetICamera()->Move(Vector3(0,1.0f,0));
        //    }
        //    if(kb->GetKeyState('s') == InputState::Pressed)
        //    {
        //        m_GraphicsManager->GetICamera()->Move(Vector3(0,-1.0f,0));
        //    }
        //    if(kb->GetKeyState('a') == InputState::Pressed)
        //    {
        //        m_GraphicsManager->GetICamera()->Move(Vector3(-1.0f,0,0));
        //    }
        //    if(kb->GetKeyState('d') == InputState::Pressed)
        //    {
        //        m_GraphicsManager->GetICamera()->Move(Vector3(1.0f,0,0));
        //    }
        //    
        //    //if(a==10)
        //    //    break;
        //       
        //    
        //    //if(mus->GetButtonState(MouseButton::RightMB)== InputState::Pressed)
        //       //m_AudioManager->PlaySoundEffect(GetFile("Electric-Bass-Low-C-Staccato.wav", AUDIO_ROOT).c_str(), 0);

        //    if(mus->GetButtonState(MouseButton::MiddleMB) == InputState::Down)
        //    {
        //        printf("X: %d\nY: %d\n", mus->getX(), mus->getY());
        //        printf("dX: %d\ndY: %d\n", mus->getdX(), mus->getdY());
        //    }
        //    
        //    m_GraphicsManager->Render();
        //}


	//Init
	m_entities = new Entity*[MAX_ENTITY_COUNT];

	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
		m_entities[i] = new Entity(i);

	EntityFactory::GetInstance()->Initialize(m_entities);
	//Init done

	//Create new Entity
	auto ball = EntityFactory::GetInstance()->CreateEntity(EntityFactory::BALL);
	ball->GetComponent<ModelComponent>()->m_modelPath = "sphere";
	ball->SetState(Entity::ACTIVATED);

	auto block = EntityFactory::GetInstance()->CreateEntity(EntityFactory::BLOCK);
	block->SetState(Entity::ACTIVATED);

	auto player = EntityFactory::GetInstance()->CreateEntity(EntityFactory::PLAYER);
	player->SetState(Entity::ACTIVATED);


	m_scoreSystem.AddPlayer(player);
	// UPDATE CALL - START


	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < MAX_ENTITY_COUNT; ++j)
		{
			if (m_entities[j]->GetState() == Entity::LIMBO)
				EntityFactory::GetInstance()->DeleteEntity(m_entities[j]);
		}


		m_movementSystem.Clear();
		m_modelSystem.Clear();
		m_scoreSystem.Clear();

		for (int j = 0; j < MAX_ENTITY_COUNT; ++j)
		{
			if (m_entities[j]->GetState() == Entity::ACTIVATED)
			{
				if (m_movementSystem.GetComponentFilter().DoesFilterPass(m_entities[j]->GetComponents()))
					m_movementSystem.Add(m_entities[j]);

				if (m_modelSystem.GetComponentFilter().DoesFilterPass(m_entities[j]->GetComponents()))
					m_modelSystem.Add(m_entities[j]);

				if (m_scoreSystem.GetComponentFilter().DoesFilterPass(m_entities[j]->GetComponents()))
					m_scoreSystem.Add(m_entities[j]);

				
			}
		}

		m_movementSystem.Update(0.5f);
		m_modelSystem.Update(0.5f);
		m_scoreSystem.Update(0.5f);

	}

	// UPDATE CALL - END
//
//	while (difftime(time(0), startTime) < 5)
//	{
//		m_GraphicsManager->Update(0.001f);
//		m_GraphicsManager->Render();
//	}
            
		m_InputManager->Update();

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
