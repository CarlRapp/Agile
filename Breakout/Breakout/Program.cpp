#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"

#include "ComponentSystem/Entity/Entity.h"
#include "ComponentSystem/System/SystemManager.h"
#include "ComponentSystem/System/ScoreSystem.h"
#include "ComponentSystem/System/MovementSystem.h"
#include "ComponentSystem/System/ModelSystem.h"
#include "ComponentSystem/EntityFactory.h"
#include "ComponentSystem/Component/HealthComponent.h"
#include "ComponentSystem/Component/PositionComponent.h"
#include "ComponentSystem/Component/RotationComponent.h"
#include "ComponentSystem/Component/ScaleComponent.h"
#include "ComponentSystem/Component/VelocityComponent.h"
#include "ComponentSystem/Component/HealthComponent.h"
#include "ComponentSystem/Component/ScoreComponent.h"
#include "ComponentSystem/Component/ModelComponent.h"
#include "ComponentSystem/Component/LifeComponent.h"

#include "ComponentSystem/World.h"

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



class MainMenu : public Scene<MainMenu>
{
private:
	VECTOR3 pos;
	SystemManager* m_systemManager;
	Entity** m_entities;
	std::map<int, Entity*> m_activeEntites;

	World* world;
public:
	MainMenu()
	{
		printf("Main Menu created!\n");
		pos = VECTOR3(0, 0, 0);
	}

	void Initialize()
	{
		printf("Initialized (MAIN MENU)\n");

		m_systemManager = new SystemManager();
		m_systemManager->AddSystem<MovementSystem>();
		//m_systemManager->AddSystem<ScoreSystem>();


		m_entities = new Entity*[MAX_ENTITY_COUNT];

		for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
			m_entities[i] = new Entity(i);

		EntityFactory::GetInstance()->Initialize(m_entities);

		for (int i = 0; i < 200; ++i)
		{
			auto ball = EntityFactory::GetInstance()->CreateEntity(EntityFactory::BALL);
			m_activeEntites.insert(std::pair<int, Entity*>(ball->GetId(), ball));
			ball->SetState(Entity::ACTIVATED);
		}


		//auto block = EntityFactory::GetInstance()->CreateEntity(EntityFactory::BLOCK);
		//m_activeEntites.insert(std::pair<int, Entity*>(block->GetId(), block));
		//block->SetState(Entity::ACTIVATED);

		//auto player = EntityFactory::GetInstance()->CreateEntity(EntityFactory::PLAYER);
		//m_activeEntites.insert(std::pair<int, Entity*>(player->GetId(), player));
		//player->SetState(Entity::ACTIVATED);

		/*	New Implementation	*/
		world = new World();
		world->AddSystem<ModelSystem>();

		Entity* e = world->CreateEntity();
		world->AddEntity(e);
		e = world->CreateEntity();
		world->AddEntity(e);
		e = world->CreateEntity();
		world->AddEntity(e);
		e = world->CreateEntity();
		e->AddComponent<PositionComponent>();
		e->AddComponent<RotationComponent>();
		e->AddComponent<ScaleComponent>();
		e->AddComponent<ModelComponent>().m_modelPath = "sphere";
		e->AddComponent<VelocityComponent>();
		world->AddEntity(e);
	}

	void LoadContent()
	{
		printf("Loading Content (MAIN MENU)\n");
	}

	void Update(float _dt)
	{
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('A') == InputState::Pressed)
			SceneManager::GetInstance()->Quit();

		world->Update(_dt);
	}
	
	void Render()
	{
		m_GraphicsManager->Render();
	}
};

int main(int argc, char** argv)
{
	printf("Application started!\n");
	
#ifdef WINDOWS
	SetWindowPos(GetConsoleWindow(), 0, 100, 0, 100, 0, SWP_NOSIZE | SWP_NOZORDER);
#endif
	/*	GRAPHICS RELATED SHIT GOES HERE	*/
	DisplayMode displayMode = DisplayMode::BorderlessWindow;
	m_GraphicsManager = GraphicsManager::GetInstance();
	m_GraphicsManager->InitWindow(100, 350, 1000, 600, displayMode);
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
