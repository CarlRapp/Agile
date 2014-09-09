#include "DXDeferred.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "DXRenderStates.h"
#include "DXEffects.h"

DXDeferred::DXDeferred(void)
{

}

DXDeferred::~DXDeferred(void)
{

}

void DXDeferred::Init(ID3D11Device *_Device, ID3D11DeviceContext *_DeviceContext, ID3D11RenderTargetView *_RenderTargetView, int _Width, int _Height)
{
	m_Device = _Device;
	m_DeviceContext = _DeviceContext;
	m_RenderTargetView = _RenderTargetView;

	m_Width = _Width;
	m_Height = _Height;


	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = (float)0;
	m_ViewPort.TopLeftY = (float)0;
	m_ViewPort.Width = (float)m_Width;
	m_ViewPort.Height = (float)m_Height;

	DXRenderStates::InitAll(m_Device);

	//InitTestTriangle();

	InitBuffers();

}

void DXDeferred::InitBuffers()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = m_Width;
	texDesc.Height = m_Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//CREATES TEXTURES
	ID3D11Texture2D* albedoTex = NULL;
	ID3D11Texture2D* depthTex = NULL;
	ID3D11Texture2D* normalSpecTex = NULL;
	ID3D11Texture2D* finalTex = NULL;


	//R16G16B16A16_UNORM
	//texDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	m_Device->CreateTexture2D(&texDesc, NULL, &normalSpecTex);

	//R8G8B8A8_UNORM
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_Device->CreateTexture2D(&texDesc, NULL, &albedoTex);

	//D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_Device->CreateTexture2D(&texDesc, NULL, &finalTex);

	//R24G8_TYPELESS & D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	m_Device->CreateTexture2D(&texDesc, NULL, &depthTex);


	//RENDER TARGETS
	m_Device->CreateRenderTargetView(albedoTex, NULL, &m_AlbedoRTV);
	m_Device->CreateRenderTargetView(normalSpecTex, NULL, &m_NormalSpecRTV);

	//GBUFFER
	GBuffer[0] = m_AlbedoRTV;
	GBuffer[1] = m_NormalSpecRTV;

	//SHADERRESOURCEVIEWS
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	//R8G8B8A8_UNORM
	m_Device->CreateShaderResourceView(albedoTex, &SRVDesc, &m_AlbedoSRV);
	m_Device->CreateShaderResourceView(finalTex, 0, &m_FinalSRV);

	//DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	m_Device->CreateShaderResourceView(depthTex, &SRVDesc, &m_DepthSRV);

	//DXGI_FORMAT_R16G16B16A16_UNORM
	SRVDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	m_Device->CreateShaderResourceView(normalSpecTex, 0, &m_NormalSpecSRV);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	m_Device->CreateDepthStencilView(depthTex, &descDSV, &m_DepthStencilView);

	//Create the UnorderedAccessView
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	m_Device->CreateUnorderedAccessView(finalTex, 0, &m_FinalUAV);


	//RELEASE
	albedoTex->Release();
	normalSpecTex->Release();
	depthTex->Release();
	finalTex->Release();
}

void DXDeferred::ClearBuffers()
{
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	float AlbedoClearColor[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, ClearColor );
	m_DeviceContext->ClearRenderTargetView(m_AlbedoRTV, AlbedoClearColor);
	m_DeviceContext->ClearRenderTargetView(m_NormalSpecRTV, ClearColor);
	m_DeviceContext->ClearUnorderedAccessViewFloat(m_FinalUAV, ClearColor);

}


void DXDeferred::Render(ID3D11RenderTargetView *_RenderTargetView)
{
	m_DeviceContext->OMSetBlendState(DXRenderStates::OpaqueBS, NULL, 0xffffffff);
	ClearBuffers();
	//FillGBuffer();
	//shadowmap->Render();
	//CombineFinal();
}