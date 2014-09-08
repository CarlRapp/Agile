#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
{
	m_IGraphics = IGraphics::GetIGraphics();
}

GraphicsManager::~GraphicsManager(void)
{

}

bool GraphicsManager::InitWindow(int x, int y, int width, int height)
{
	return m_IGraphics->InitWindow(x, y, width, height);
}

void GraphicsManager::Render()
{
	return m_IGraphics->Render();
}