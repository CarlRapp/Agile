#pragma once
#ifndef GLGRAPHICS_H
#define GLGRAPHICS_H

#include "../IGraphics.h"
#include "GLWindow.h"

class GLGraphics : public IGraphics
{

private:
	GLWindow *m_Window;


public:

	GLGraphics(void);
	~GLGraphics(void);

	bool  InitWindow(int width, int height);
        void Render();
};



#endif