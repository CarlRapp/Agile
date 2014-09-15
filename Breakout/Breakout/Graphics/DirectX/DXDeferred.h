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


	//ID3D11Texture2D			*m_depthStencil = NULL;
	//ID3D11DepthStencilView	*m_depthStencilView = NULL;
	ID3D11Device			*m_device = NULL;
	ID3D11DeviceContext		*m_deviceContext = NULL;

	D3D11_VIEWPORT			m_viewPort;

	int						m_width, m_height;

	ID3D11DepthStencilView		*m_depthStencilView, *m_shadowMapDSV;
	ID3D11RenderTargetView		*m_albedoRTV, *m_normalSpecRTV;
	ID3D11ShaderResourceView	*m_albedoSRV, *m_normalSpecSRV, *m_depthSRV, *m_finalSRV;

	ID3D11UnorderedAccessView	*m_finalUAV;

	ID3D11RenderTargetView* m_GBuffer[2];

	//TEST
	DXMesh					*m_testTriangle;
	void					RenderQuad(D3D11_VIEWPORT &_vp, ID3D11ShaderResourceView* _SRV, ID3DX11EffectTechnique* _tech);
	ID3D11Buffer			*m_fullSceenQuad;
	void					InitFullScreenQuad();


	void					InitBuffers();

	void					ClearBuffers();
	void					FillGBuffer(ICamera* _camera);
	void					CombineFinal(ID3D11RenderTargetView *_renderTargetView);

	void					InitTestTriangle();
	void					RenderTestTriangle(ICamera* _camera);


public:

	DXDeferred(void);
	~DXDeferred(void);

	void Init(ID3D11Device *_device, ID3D11DeviceContext *_deviceContext, int _width, int _height);

	void	Render(ID3D11RenderTargetView *_renderTargetView, ICamera* _camera);

};



#endif