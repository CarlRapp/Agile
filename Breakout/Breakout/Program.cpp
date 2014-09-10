#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <time.h>
GraphicsManager *m_GraphicsManager;

int main(int argc, char** argv)
{
        time_t startTime = time(0);
        
        double secondsElapsed;
    
	m_GraphicsManager = new GraphicsManager();
	m_GraphicsManager->InitWindow(600, 400);
        m_GraphicsManager->Init3D();
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