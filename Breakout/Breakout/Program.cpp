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
#include "Scenes/PowerPointScene.h"

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
FileManager* m_fileManager;
SceneManager* m_SceneManager;
EntityFactory* m_EntityFactory;

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720

int main(int argc, char** argv)
{
	printf("Application started!\n");
	

#ifdef WINDOWS
	SetWindowPos(GetConsoleWindow(), 0, 100, 0, 100, 0, SWP_NOSIZE | SWP_NOZORDER);
#endif
	/*	GRAPHICS RELATED SHIT GOES HERE	*/
	DisplayMode displayMode = DisplayMode::Fullscreen;
	m_GraphicsManager = GraphicsManager::GetInstance();
	m_GraphicsManager->InitWindow(0, 0, SCREENWIDTH, SCREENHEIGHT, displayMode);
	m_GraphicsManager->Init3D(displayMode);
        
        std::vector<std::string> files;
	FileManager::GetInstance().GetFilesInDirectory(files, GetFile("", MODEL_ROOT));
	for (auto file : files)
	{
            file = file.substr(0, file.size() - 4);
            GraphicsManager::GetInstance()->GetIGraphics()->LoadModel(file);
	}


	files.clear();
	FileManager::GetInstance().GetFilesInDirectory(files, GetFile("", TEXTURE_ROOT));

	for (auto file : files)
	{
		GraphicsManager::GetInstance()->GetIGraphics()->LoadTexture(file);
	}

	/*	AUDIO RELATED SHIT GOES HERE	*/
	m_AudioManager = AudioManager::GetInstance();

	/*	INPUT RELATED SHIT GOES HERE	*/
	InputManager::Init(SCREENWIDTH, SCREENHEIGHT);
	m_InputManager = InputManager::GetInstance();

	m_EntityFactory = EntityFactory::GetInstance();
	m_fileManager   = &FileManager::GetInstance();
	/*	CREATE SCENE MANAGER HERE	*/
	m_SceneManager = SceneManager::GetInstance();
	m_SceneManager->AddScene<MainMenuScene>(false);
	m_SceneManager->AddScene<GameScene>(false);
	m_SceneManager->AddScene<GameOverScene>(false);
	m_SceneManager->AddScene<PowerPointScene>(false);
        
	m_SceneManager->ChangeScene<PowerPointScene>();

	/*	Random Seed	*/
	printf("!!!IMPORTANT INFO!!!\n");
	printf("Program.cpp is currently setting the random seed to 0 every time!!!!\n");
	printf("!!!IMPORTANT INFO!!!\n");
	srand(0);

	/*	START HERE	*/
	m_SceneManager->Start();

	m_SceneManager->Quit();

	
	//SafeDelete(m_GraphicsManager);
	SafeDelete(m_AudioManager);
	SafeDelete(m_InputManager);
	SafeDelete(m_fileManager);
	SafeDelete(m_EntityFactory);
    SafeDelete(m_SceneManager);
        
	//_CrtDumpMemoryLeaks();
	return 0;
}
