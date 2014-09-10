#pragma once
#ifndef DXDEFERRED_H
#define DXDEFERRED_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "../IGraphics.h"
#include "DXWindow.h"

class DXDeferred
{

private:
	DXWindow				*m_Window;


	//ID3D11Texture2D			*m_DepthStencil = NULL;
	//ID3D11DepthStencilView	*m_DepthStencilView = NULL;
	ID3D11Device			*m_Device = NULL;
	ID3D11DeviceContext		*m_DeviceContext = NULL;


	HRESULT					InitDirect3D();

	void					FillGBuffer();
	void					CombineFinal();


public:

	DXDeferred(void);
	~DXDeferred(void);

	void Init(ID3D11Device *_Device, ID3D11DeviceContext *_DeviceContext);

	void	Render(ID3D11RenderTargetView *_RenderTargetView);

};



#endif