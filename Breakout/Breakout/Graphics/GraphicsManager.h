#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "IGraphics.h"
#include "ICamera.h"
#include "../stdafx.h"

#ifdef WINDOWS
#include <windows.h>
#endif

#ifdef DIRECTX
#include <D3D11.h>
#include <d3dCompiler.h>
#endif



class GraphicsManager
{
private:
	GraphicsManager();
	static GraphicsManager* m_GMInstance;

	IGraphics *m_IGraphics;
	ICamera	*m_ICamera;

public:

	~GraphicsManager(void);
	static GraphicsManager* GetInstance();


	bool        InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode);
	IGraphics*  GetIGraphics() { return m_IGraphics; }
	void        Render();
	void        Update();
    ICamera*    GetICamera();

	//AddObject()
	//RemoveObject()

	bool        Init3D(DisplayMode _displayMode);
};



#endif