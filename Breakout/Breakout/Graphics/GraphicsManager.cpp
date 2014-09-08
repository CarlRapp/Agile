#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::m_gmInstance = 0;

GraphicsManager::GraphicsManager(void)
{
	m_IGraphics = IGraphics::GetIGraphics();
}

GraphicsManager* GraphicsManager::GetInstance()
{
	if (m_gmInstance)
		return m_gmInstance;

	m_gmInstance = new GraphicsManager();

	return m_gmInstance;
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