#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>
#include <SDL.h>

GraphicsManager *m_GraphicsManager;

int main(int argc, char** argv)
{
	m_GraphicsManager = GraphicsManager::GetInstance();
	m_GraphicsManager->InitWindow(600, 400);

        char f;
        std::string hay;
        hay = "LOL";
        std::cout << hay;
        std::cin >> f;
	return 0;
}