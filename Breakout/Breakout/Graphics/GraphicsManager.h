#pragma once
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

class GraphicsManager
{
private:
	GraphicsManager();
	static GraphicsManager* m_gmInstance;

	IGraphics *m_IGraphics;

public:
	~GraphicsManager(void);
	static GraphicsManager* GetInstance();


	bool  InitWindow(int width, int height);
	IGraphics* GetIGraphics() { return m_IGraphics; }
};



#endif