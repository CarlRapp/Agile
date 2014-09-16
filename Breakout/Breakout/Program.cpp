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

#include "Scene/SceneManager.h"
using namespace SceneSystem;

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;
InputManager* m_InputManager; 
SceneManager* m_SceneManager;

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

class MainMenu : public Scene<MainMenu>
{
private:
	Vector3 pos;
public:
	MainMenu()
	{
		printf("Main Menu created!\n");
		pos = Vector3(0, 0, 0);
	}

	void Initialize()
	{
		printf("Initialized (MAIN MENU)\n");
	}

	void LoadContent()
	{
		printf("Loading Content (MAIN MENU)\n");
	}

	void Update(float _dt)
	{
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('A') == InputState::Pressed)
			SceneManager::GetInstance()->Quit();
			
	}
	
	void Render()
	{
		m_GraphicsManager->Render();
	}
};

int main(int argc, char** argv)
{
	printf("Application started!\n");
	
	/*	GRAPHICS RELATED SHIT GOES HERE	*/
	DisplayMode displayMode = DisplayMode::BorderlessWindow;
	m_GraphicsManager = GraphicsManager::GetInstance();
	m_GraphicsManager->InitWindow(600, 100, 1000, 800, displayMode);
	m_GraphicsManager->Init3D(displayMode);

	/*	AUDIO RELATED SHIT GOES HERE	*/
	AudioManager::GetInstance();

	/*	INPUT RELATED SHIT GOES HERE	*/
	InputManager::GetInstance();

	/*	CREATE SCENE MANAGER HERE	*/
	m_SceneManager = SceneManager::GetInstance();
	if (!m_SceneManager->AddScene<MainMenu>(true))
		printf("Could not add scene!\n");

	/*	START HERE	*/
	m_SceneManager->Start();


	return 0;
}
