#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"

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
public:
	MainMenu()
	{
		printf("Main Menu created!\n");
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
