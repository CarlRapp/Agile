#pragma once
#ifndef IGRAPHICS_H
#define IGRAPHICS_H


class IGraphics
{
private:


public:

	virtual ~IGraphics(void) {};

	virtual bool InitWindow(int x, int y, int width, int height) = 0;

	virtual void Render() = 0;

	static IGraphics* GetIGraphics();

};




#endif