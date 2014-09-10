#pragma once
#ifndef IGRAPHICS_H
#define IGRAPHICS_H

#include "ICamera.h"


enum DisplayMode
{
	Fullscreen, Windowed, BorderlessWindow
};

class IGraphics
{
private:


public:


	virtual ~IGraphics(void) {};

	virtual bool InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode) = 0;

	virtual void Render(ICamera* _Camera) = 0;

	static IGraphics* GetIGraphics();

};




#endif