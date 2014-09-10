#include "ICamera.h"

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
        return new GLCamera();
#endif
}