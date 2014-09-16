#include "ICamera.h"

#ifdef DIRECTX
#include "DirectX\DXCamera.h"
#else
#include "OpenGL/GLCamera.h"
#endif

ICamera* ICamera::GetICamera(float _fov, int _width,int _height, float _nearZ, float _farZ)
{
    
#ifdef DIRECTX
	return new DXCamera();
#else   //0.5f, 300.0f, 16 / 9.0f, 90
        return new GLCamera(_fov,_width,_height,_nearZ,_farZ);
#endif
}