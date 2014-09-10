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

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;
InputManager* m_InputManager;


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
