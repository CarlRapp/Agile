#include "Graphics\GraphicsManager.h"

GraphicsManager *m_GraphicsManager;

int main(int argc, char** argv)
{
	m_GraphicsManager = new GraphicsManager();
	m_GraphicsManager->InitWindow(600, 400);

	system("pause");

	return 0;
}