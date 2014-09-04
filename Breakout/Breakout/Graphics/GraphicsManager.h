#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "IGraphics.h"

class GraphicsManager
{
private:

	IGraphics *m_IGraphics;

public:

	GraphicsManager();
	~GraphicsManager(void);

	bool  InitWindow(int width, int height);
};



#endif