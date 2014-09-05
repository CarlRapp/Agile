#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#ifdef WINDOWS
#include <windows.h>
#endif

#ifdef DIRECTX
#include <D3D11.h>
#include <d3dCompiler.h>
#endif

#include "IGraphics.h"
#include "OpenGL/GLGraphics.h"

class GraphicsManager
{
private:

	IGraphics* m_IGraphics;

public:

	GraphicsManager();
	~GraphicsManager(void);

	bool  InitWindow(int width, int height);
        IGraphics* GetInstance();
        void Render();
};



#endif