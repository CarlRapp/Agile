#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"

#include "Scenes/GameScene.h"

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
	if (!m_SceneManager->AddScene<GameScene>(true))
		printf("Could not add scene!\n");

	/*	START HERE	*/
	m_SceneManager->Start();

	GraphicsManager::Destroy();

	return 0;
}
