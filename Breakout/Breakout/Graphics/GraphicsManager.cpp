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

bool GraphicsManager::InitWindow(int x, int y, int width, int height, DisplayMode _displayMode)
{
	m_ICamera = ICamera::GetICamera();
	m_ICamera->SetPosition(Vector3(0, 0, -20));
	m_ICamera->SetViewPort(0, 0, width, height);
	return m_IGraphics->InitWindow(x, y, width, height, _displayMode);
}


void GraphicsManager::Render()
{
	return m_IGraphics->Render(m_ICamera);
}

bool GraphicsManager::Init3D()
{
    m_IGraphics->Init3D();
}