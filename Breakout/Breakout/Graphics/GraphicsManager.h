#pragma once
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#ifdef WINDOWS
#include <windows.h>
#endif

#include "IGraphics.h"

class GraphicsManager
{
private:
	GraphicsManager();
	static GraphicsManager* m_gmInstance;

	IGraphics *m_IGraphics;

	//lista på objekt
public:

	~GraphicsManager(void);
	static GraphicsManager* GetInstance();


	bool	InitWindow(int x, int y, int width, int height);
	IGraphics* GetIGraphics() { return m_IGraphics; }
	void	Render();

	//AddObject()
	//RemoveObject()

};



#endif