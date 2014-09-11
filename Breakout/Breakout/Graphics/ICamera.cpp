#include "ICamera.h"

#ifdef DIRECTX
#include "DirectX\DXCamera.h"
#else
#include "OpenGL/GLCamera.h"
#endif

ICamera* ICamera::GetICamera()
{
    
#ifdef DIRECTX
	return new DXCamera();
#else
        return new GLCamera(1,1,1,1,1);
#endif
}