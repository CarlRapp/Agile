#ifndef IGRAPHICS_H
#define IGRAPHICS_H

class IGraphics
{
private:
        IGraphics* m_graphics;

public:

	virtual ~IGraphics(void) {};

	virtual bool InitWindow(int width, int height) = 0;
        virtual bool Init3D()=0;
	IGraphics* GetIGraphics();
        virtual void Render();
};




#endif