#ifndef IGRAPHICS_H
#define IGRAPHICS_H
#include "../stdafx.h"
#include "ICamera.h"


enum DisplayMode
{
	Fullscreen, Windowed, BorderlessWindow
};

class IGraphics
{
private:
        IGraphics* m_graphics;

public:


	virtual ~IGraphics(void){};

	virtual bool InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode) = 0;
	virtual bool Init3D(DisplayMode _displayMode) = 0;
	//IGraphics* GetIGraphics();


	virtual void Render(ICamera* _Camera) = 0;

	static IGraphics* GetIGraphics();
};




#endif