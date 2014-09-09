#include "ICamera.h"
#define DIRECTX
#ifdef DIRECTX
#include "DirectX\DXCamera.h"
#else
#include "OpenGL/GLGraphics.h"
#endif

ICamera* ICamera::GetICamera()
{
    
#ifdef DIRECTX
	return new DXCamera();
#else
        return new GLGraphics();
#endif
}