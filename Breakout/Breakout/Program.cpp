#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Storage/FileManager.h"

GraphicsManager *m_GraphicsManager;
AudioManager* m_AudioManager;
InputManager* m_InputManager;


int main(int argc, char** argv)
{

        time_t startTime = time(0);
        
        double secondsElapsed;

        m_GraphicsManager = GraphicsManager::GetInstance();

	m_GraphicsManager->InitWindow(100, 100, 400, 400);

        m_GraphicsManager->Init3D();

        
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
