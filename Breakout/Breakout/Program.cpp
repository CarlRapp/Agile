#include "./Graphics/GraphicsManager.h"
#include <iostream>
#include <string>

GraphicsManager *m_GraphicsManager;

int main(int argc, char** argv)
{
	m_GraphicsManager = new GraphicsManager();
	m_GraphicsManager->InitWindow(100, 100, 400, 100);

        char f;
        std::string hay;
        hay = "LOL";
        std::cout << hay;
        std::cin >> f;
	return 0;
}