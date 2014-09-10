#pragma once
#ifndef DXGRAPHICS_H
#define DXGRAPHICS_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "../IGraphics.h"
#include "DXWindow.h"
#include "DXDeferred.h"
#include "../ICamera.h"

class DXGraphics : public IGraphics
{

private:
	DXWindow				*m_Window;


	IDXGISwapChain			*m_SwapChain = NULL;
	ID3D11RenderTargetView	*m_RenderTargetView = NULL;

	ID3D11Texture2D			*m_DepthStencil = NULL;
	ID3D11DepthStencilView	*m_DepthStencilView = NULL;
	ID3D11Device			*m_Device = NULL;
	ID3D11DeviceContext		*m_DeviceContext = NULL;

	DXDeferred				*m_DXDeferred = NULL;


	HRESULT					InitDirect3D(DisplayMode _displayMode);

	int m_Width, m_Height;



public:

	DXGraphics(void);
	~DXGraphics(void);

	bool  InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode);
	bool  Init3D(DisplayMode _displayMode);
	DXWindow* GetWindow(){ return m_Window; }
	void	Render(ICamera* _Camera);
};



#endif