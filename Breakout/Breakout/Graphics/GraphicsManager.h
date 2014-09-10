#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#ifdef WINDOWS
#include <windows.h>
#endif

#include "IGraphics.h"
#include "ICamera.h"

class GraphicsManager
{
private:
	GraphicsManager();
	static GraphicsManager* m_gmInstance;

	IGraphics *m_IGraphics;
	ICamera	*m_ICamera;

	//lista på objekt
public:

	~GraphicsManager(void);
	static GraphicsManager* GetInstance();


	bool	InitWindow(int x, int y, int width, int height, DisplayMode _displayMode);
	IGraphics* GetIGraphics() { return m_IGraphics; }
	void	Render();

	//AddObject()
	//RemoveObject()

};



#endif