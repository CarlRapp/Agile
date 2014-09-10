#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::m_gmInstance = 0;

GraphicsManager::GraphicsManager(void)
{
#ifdef WINDOWS
    m_IGraphics = new DXGraphics();
#else 
    m_IGraphics = new GLGraphics();
#endif
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
    m_IGraphics->Render();
}

bool GraphicsManager::Init3D()
{
    m_IGraphics->Init3D();
}