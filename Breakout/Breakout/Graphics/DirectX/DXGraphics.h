#pragma once
#ifndef DXGRAPHICS_H
#define DXGRAPHICS_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "../IGraphics.h"
#include "DXWindow.h"

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


	HRESULT					InitDirect3D();



public:

	DXGraphics(void);
	~DXGraphics(void);

	bool  InitWindow(int _X, int _Y, int _Width, int _Height);
	DXWindow* GetWindow(){ return m_Window; }
	void	Render();
};



#endif