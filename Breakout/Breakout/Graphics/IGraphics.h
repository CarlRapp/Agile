#ifndef IGRAPHICS_H
#define IGRAPHICS_H
#include "../stdafx.h"

class IGraphics
{
private:
        IGraphics* m_graphics;

public:

	virtual ~IGraphics(void){};

        virtual bool Init3D()=0;
	//IGraphics* GetIGraphics();

	virtual bool InitWindow(int x, int y, int width, int height)=0;

	virtual void Render()=0;

	static IGraphics* GetIGraphics();
};




#endif