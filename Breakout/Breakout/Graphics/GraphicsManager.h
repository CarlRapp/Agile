#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "IGraphics.h"
#include "ICamera.h"


#ifdef WINDOWS
#include <windows.h>
#endif

#ifdef DIRECTX
#include <D3D11.h>
#include <d3dCompiler.h>
#endif

#ifdef OPENGL
#include "OpenGL/GLGraphics.h"
#endif



class GraphicsManager
{
private:
	GraphicsManager();
	static GraphicsManager* m_gmInstance;

	IGraphics *m_IGraphics;
	ICamera	*m_ICamera;

public:

	~GraphicsManager(void);
	static GraphicsManager* GetInstance();


	bool	InitWindow(int x, int y, int width, int height, DisplayMode _displayMode);
	IGraphics* GetIGraphics() { return m_IGraphics; }
	void	Render();

	//AddObject()
	//RemoveObject()

        bool        Init3D();
};



#endif