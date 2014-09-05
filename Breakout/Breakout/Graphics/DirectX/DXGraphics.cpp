#include "DXGraphics.h"

DXGraphics::DXGraphics(void)
{

}

DXGraphics::~DXGraphics(void)
{

}

bool DXGraphics::InitWindow(int width, int height)
{
	m_Window = new DXWindow();
	return m_Window->InitWindow(width, height);
}