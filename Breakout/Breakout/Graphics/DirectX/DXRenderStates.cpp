
#include "DXRenderStates.h"

ID3D11RasterizerState*   DXRenderStates::WireframeRS       = 0;
ID3D11RasterizerState*   DXRenderStates::NoCullRS = 0;
	 
ID3D11DepthStencilState* DXRenderStates::EqualsDSS = 0;
ID3D11DepthStencilState* DXRenderStates::LessDSS = 0;
ID3D11DepthStencilState* DXRenderStates::NoDSS = 0;

ID3D11BlendState*        DXRenderStates::OpaqueBS = 0;
ID3D11BlendState*        DXRenderStates::AlphaToCoverageBS = 0;
ID3D11BlendState*        DXRenderStates::TransparentBS = 0;

void DXRenderStates::InitAll(ID3D11Device* device)
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

	device->CreateRasterizerState(&wireframeDesc, &WireframeRS);

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&noCullDesc, &NoCullRS);

	//
	// EqualsDSS
	//
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;

	device->CreateDepthStencilState(&equalsDesc, &EqualsDSS);

	//
	// LessDSS
	//
	D3D11_DEPTH_STENCIL_DESC lessDesc;
	ZeroMemory(&lessDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	lessDesc.DepthEnable = true;
	lessDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	lessDesc.DepthFunc = D3D11_COMPARISON_LESS;

	device->CreateDepthStencilState(&lessDesc, &LessDSS);

	//
	// NoDSS
	//
	D3D11_DEPTH_STENCIL_DESC noDesc;
	ZeroMemory(&noDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	noDesc.DepthEnable = false;
	noDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	noDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&noDesc, &NoDSS);



	//
	// OpaqueBS
	//

	D3D11_BLEND_DESC opaqueDesc = {0};
	opaqueDesc.AlphaToCoverageEnable = false;
	opaqueDesc.IndependentBlendEnable = false;
	opaqueDesc.RenderTarget[0].BlendEnable = false;
	opaqueDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&opaqueDesc, &OpaqueBS);



	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = {0};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS);

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

	device->CreateBlendState(&transparentDesc, &TransparentBS);
}

void DXRenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(EqualsDSS);
	ReleaseCOM(LessDSS);
	ReleaseCOM(NoDSS);
	ReleaseCOM(OpaqueBS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
}