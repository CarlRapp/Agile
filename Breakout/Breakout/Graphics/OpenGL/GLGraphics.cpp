#include "GLGraphics.h"

GLGraphics::GLGraphics(void)
{

}

GLGraphics::~GLGraphics(void)
{

}

bool GLGraphics::InitWindow(int width, int height)
{
	m_Window = new GLWindow();
	return m_Window->InitWindow(width, height);
}