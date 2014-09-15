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
#else   //0.5f, 300.0f, 16 / 9.0f, 90
        return new GLCamera(90,16,9,0.1f,100);
#endif
}