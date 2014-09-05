#include "IGraphics.h"
#define DIRECTX
#ifdef DIRECTX
#include "DirectX\DXGraphics.h"
#else
#include "OpenGL/GLGraphics.h"
#endif

IGraphics* IGraphics::GetIGraphics()
{
    
#ifdef DIRECTX
	return new DXGraphics();
#else
        return new GLGraphics();
#endif
}