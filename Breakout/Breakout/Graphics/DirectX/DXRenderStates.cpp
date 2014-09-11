
#include "DXRenderStates.h"

ID3D11RasterizerState*   DXRenderStates::m_wireframeRS       = 0;
ID3D11RasterizerState*   DXRenderStates::m_noCullRS = 0;
	 
ID3D11DepthStencilState* DXRenderStates::m_equalsDSS = 0;
ID3D11DepthStencilState* DXRenderStates::m_lessDSS = 0;
ID3D11DepthStencilState* DXRenderStates::m_noDSS = 0;

ID3D11BlendState*        DXRenderStates::m_opaqueBS = 0;
ID3D11BlendState*        DXRenderStates::m_alphaToCoverageBS = 0;
ID3D11BlendState*        DXRenderStates::m_transparentBS = 0;

void DXRenderStates::InitAll(ID3D11Device* _device)
{
	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	_device->CreateRasterizerState(&wireframeDesc, &m_wireframeRS);

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	_device->CreateRasterizerState(&noCullDesc, &m_noCullRS);

	//
	// EqualsDSS
	//
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;

	_device->CreateDepthStencilState(&equalsDesc, &m_equalsDSS);

	//
	// LessDSS
	//
	D3D11_DEPTH_STENCIL_DESC lessDesc;
	ZeroMemory(&lessDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	lessDesc.DepthEnable = true;
	lessDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	lessDesc.DepthFunc = D3D11_COMPARISON_LESS;

	_device->CreateDepthStencilState(&lessDesc, &m_lessDSS);

	//
	// NoDSS
	//
	D3D11_DEPTH_STENCIL_DESC noDesc;
	ZeroMemory(&noDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	noDesc.DepthEnable = false;
	noDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	noDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	_device->CreateDepthStencilState(&noDesc, &m_noDSS);



	//
	// OpaqueBS
	//

	D3D11_BLEND_DESC opaqueDesc = {0};
	opaqueDesc.AlphaToCoverageEnable = false;
	opaqueDesc.IndependentBlendEnable = false;
	opaqueDesc.RenderTarget[0].BlendEnable = false;
	opaqueDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&opaqueDesc, &m_opaqueBS);



	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = {0};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&alphaToCoverageDesc, &m_alphaToCoverageBS);

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&transparentDesc, &m_transparentBS);
}

void DXRenderStates::DestroyAll()
{
	ReleaseCOM(m_wireframeRS);
	ReleaseCOM(m_noCullRS);
	ReleaseCOM(m_equalsDSS);
	ReleaseCOM(m_lessDSS);
	ReleaseCOM(m_noDSS);
	ReleaseCOM(m_opaqueBS);
	ReleaseCOM(m_alphaToCoverageBS);
	ReleaseCOM(m_transparentBS);
}