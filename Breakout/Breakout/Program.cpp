#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>

GraphicsManager *m_GraphicsManager;

int main(int argc, char** argv)
{
    
	m_GraphicsManager = new GraphicsManager();
	m_GraphicsManager->InitWindow(600, 400);
        
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