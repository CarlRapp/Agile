#pragma once
#ifndef DXGRAPHICS_H
#define DXGRAPHICS_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "../IGraphics.h"
#include "DXWindow.h"

class DXGraphics : public IGraphics
{

private:
	DXWindow *m_Window;


public:

	DXGraphics(void);
	~DXGraphics(void);

	bool  InitWindow(int width, int height);
	DXWindow* GetWindow(){ return m_Window; }
};



#endif