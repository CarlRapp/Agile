#pragma once
#ifndef DXDEFERRED_H
#define DXDEFERRED_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "../IGraphics.h"
#include "DXWindow.h"
#include "../ICamera.h"
#include "DXMesh.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "DXRenderStates.h"
#include "DXEffects.h"
#include "DXVertex.h"

class DXDeferred
{

private:
	DXWindow				*m_Window;


	//ID3D11Texture2D			*m_DepthStencil = NULL;
	//ID3D11DepthStencilView	*m_DepthStencilView = NULL;
	ID3D11Device			*m_Device = NULL;
	ID3D11DeviceContext		*m_DeviceContext = NULL;

	D3D11_VIEWPORT			m_ViewPort;

	int						m_Width, m_Height;

	ID3D11DepthStencilView		*m_DepthStencilView, *m_ShadowMapDSV;
	ID3D11RenderTargetView		*m_AlbedoRTV, *m_NormalSpecRTV;
	ID3D11ShaderResourceView	*m_AlbedoSRV, *m_NormalSpecSRV, *m_DepthSRV, *m_FinalSRV;

	ID3D11UnorderedAccessView	*m_FinalUAV;

	ID3D11RenderTargetView* GBuffer[2];

	//TEST
	DXMesh					*m_TestTriangle;
	void					RenderQuad(D3D11_VIEWPORT &vp, ID3D11ShaderResourceView* SRV, ID3DX11EffectTechnique* tech);
	ID3D11Buffer			*m_FullSceenQuad;
	void					InitFullScreenQuad();


	void					InitBuffers();

	void					ClearBuffers();
	void					FillGBuffer(ICamera* _Camera);
	void					CombineFinal(ID3D11RenderTargetView *_RenderTargetView);

	void					InitTestTriangle();
	void					RenderTestTriangle(ICamera* _Camera);


public:

	DXDeferred(void);
	~DXDeferred(void);

	void Init(ID3D11Device *_Device, ID3D11DeviceContext *_DeviceContext, int _Width, int _Height);

	void	Render(ID3D11RenderTargetView *_RenderTargetView, ICamera* _Camera);

};



#endif