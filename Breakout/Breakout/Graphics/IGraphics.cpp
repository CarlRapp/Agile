#include "IGraphics.h"
#include "DirectX\DXGraphics.h"

IGraphics* IGraphics::GetIGraphics()
{
	return new DXGraphics();
}