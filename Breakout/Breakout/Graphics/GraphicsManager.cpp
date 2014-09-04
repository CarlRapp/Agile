#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
{
	m_IGraphics = IGraphics::GetIGraphics();
}

GraphicsManager::~GraphicsManager(void)
{

}

bool GraphicsManager::InitWindow(int width, int height)
{
	return m_IGraphics->InitWindow(width, height);
}