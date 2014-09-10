#include "DXDeferred.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


DXDeferred::DXDeferred(void)
{

}

DXDeferred::~DXDeferred(void)
{

}

void DXDeferred::Init(ID3D11Device *_Device, ID3D11DeviceContext *_DeviceContext)
{
	m_Device = _Device;
	m_DeviceContext = _DeviceContext;
}


void DXDeferred::Render(ID3D11RenderTargetView *_RenderTargetView)
{
	//FillGBuffer();
	//shadowmap->Render();
	//CombineFinal();
}