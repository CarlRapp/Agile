#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::m_GMInstance = 0;

GraphicsManager::GraphicsManager(void)
{
	m_IGraphics = IGraphics::GetIGraphics();
}

GraphicsManager* GraphicsManager::GetInstance()
{
	if (m_GMInstance)
		return m_GMInstance;

	m_GMInstance = new GraphicsManager();

	return m_GMInstance;
}

GraphicsManager::~GraphicsManager(void)
{
    SafeDelete(m_IGraphics);
}

bool GraphicsManager::InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode)
{

	m_ICamera = ICamera::GetICamera(45.0f,_width,_height,0.1f,100.0f);
	//m_ICamera->SetPosition(Vector3(0, 1, 0));

	//m_ICamera->SetLookAt(Vector3(0,0,0));

	m_ICamera->SetViewPort(0, 0, _width, _height);
	return m_IGraphics->InitWindow(_x, _y, _width, _height, _displayMode);

}


void GraphicsManager::Render()
{
	return m_IGraphics->Render(m_ICamera);
}

float test = 0.0f;
void GraphicsManager::Update(double dt)
{
}


bool GraphicsManager::Init3D(DisplayMode _displayMode)
{
    return m_IGraphics->Init3D(_displayMode);
}

ICamera* GraphicsManager::GetICamera()
{
    return m_ICamera;
}