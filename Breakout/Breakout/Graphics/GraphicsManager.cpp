#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
{
#ifdef WINDOWS
    m_IGraphics = new DXGraphics();
#else 
    m_IGraphics = new GLGraphics();
#endif
}

GraphicsManager::~GraphicsManager(void)
{

}

bool GraphicsManager::InitWindow(int width, int height)
{
    return m_IGraphics->InitWindow(width, height);
}

IGraphics* GraphicsManager::GetInstance()
{
    return m_IGraphics;
}

void GraphicsManager::Render()
{
    m_IGraphics->Render();
}

bool GraphicsManager::Init3D()
{
    m_IGraphics->Init3D();
}