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
        
        /*  INPUT MANAGER   */
        m_InputManager = InputManager::GetInstance();
        Keyboard* kb = m_InputManager->getInputDevices()->GetKeyboard();
        Mouse* mus = m_InputManager->getInputDevices()->GetMouse();

	//m_AudioManager->PlaySoundEffect("Kettle-Drum-1.wav", -1);

	//Mix_Chunk* chunk = FileManager::GetInstance().LoadSoundEffect("Kettle-Drum-1.wav");
        //m_AudioManager->PlayMusic(GetFile("Kettle-Drum-1.wav", AUDIO_ROOT).c_str(), -1);
        int a = 0, b = 0, c = 0;
        while(difftime(time(0),startTime)<10)
        {
            m_InputManager->Update();
            if(kb->GetKeyState('1') == InputState::Pressed)
            {
                printf("a clicked %d\n", ++a);
                //m_AudioManager->PlaySoundEffect(GetFile("Kettle-Drum-1.wav", AUDIO_ROOT).c_str(), 0);
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
