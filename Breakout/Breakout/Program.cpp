#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"

#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"
#include "Scenes/GameOverScene.h"

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
	m_GraphicsManager->InitWindow(100, 350, 1280, 720, displayMode);
	m_GraphicsManager->Init3D(displayMode);

	/*	AUDIO RELATED SHIT GOES HERE	*/
	AudioManager::GetInstance();

	/*	INPUT RELATED SHIT GOES HERE	*/
	InputManager::GetInstance();

	/*	CREATE SCENE MANAGER HERE	*/
	m_SceneManager = SceneManager::GetInstance();
	m_SceneManager->AddScene<MainMenuScene>(false);
	m_SceneManager->AddScene<GameScene>(false);
	m_SceneManager->AddScene<GameOverScene>(false);
        
	m_SceneManager->ChangeScene<MainMenuScene>();

	/*	Random Seed	*/
	printf("!!!IMPORTANT INFO!!!\n");
	printf("Program.cpp is currently setting the random seed to 0 every time!!!!\n");
	printf("!!!IMPORTANT INFO!!!\n");
	srand(0);

	/*	START HERE	*/
	m_SceneManager->Start();

	return 0;
}
