#include "DXDeferred.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "DXRenderStates.h"
#include "DXEffects.h"
#include "DXVertex.h"

DXDeferred::DXDeferred(void)
{

}

DXDeferred::~DXDeferred(void)
{

}

void DXDeferred::Init(ID3D11Device *_Device, ID3D11DeviceContext *_DeviceContext, int _Width, int _Height)
{
	m_Device = _Device;
	m_DeviceContext = _DeviceContext;

	m_Width = _Width;
	m_Height = _Height;


	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = (float)0;
	m_ViewPort.TopLeftY = (float)0;
	m_ViewPort.Width = (float)m_Width;
	m_ViewPort.Height = (float)m_Height;

	DXRenderStates::InitAll(m_Device);
	DXEffects::InitAll(m_Device);
	DXInputLayouts::InitAll(m_Device);

	InitTestTriangle();
	InitFullScreenQuad();
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
	float AlbedoClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, ClearColor );
	m_DeviceContext->ClearRenderTargetView(m_AlbedoRTV, AlbedoClearColor);
	m_DeviceContext->ClearRenderTargetView(m_NormalSpecRTV, ClearColor);
	m_DeviceContext->ClearUnorderedAccessViewFloat(m_FinalUAV, ClearColor);

}

void DXDeferred::FillGBuffer(ICamera* _Camera)
{
	m_DeviceContext->OMSetRenderTargets(2, GBuffer, m_DepthStencilView);


	m_DeviceContext->OMSetDepthStencilState(DXRenderStates::LessDSS, 0);

	m_DeviceContext->RSSetState(DXRenderStates::NoCullRS);
	//m_DeviceContext->RSSetState(DXRenderStates::WireframeRS);
	RenderTestTriangle(_Camera);

	m_DeviceContext->OMSetRenderTargets(0, 0, 0);
}

void DXDeferred::CombineFinal(ID3D11RenderTargetView *_RenderTargetView)
{
	m_DeviceContext->RSSetState(DXRenderStates::NoCullRS);
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
	m_DeviceContext->OMSetRenderTargets(1, &_RenderTargetView, NULL);
	m_DeviceContext->OMSetDepthStencilState(DXRenderStates::NoDSS, 0);

	//DXEffects::CombineFinalFX->SetTexture(m_FinalSRV);
	//Effects::CombineFinalFX->SetTexture(m_ShadowMapSRV0);


	RenderQuad(m_ViewPort, m_AlbedoSRV, DXEffects::CombineFinalFX->ColorTech);
	//RenderQuad(m_ViewPort, m_NormalSpecSRV, Effects::CombineFinalFX->ColorTech);
	//Effects::CombineFinalFX->SetOpacity(0.8f);
	//RenderQuad(shadowVP, m_ShadowMapSRV, Effects::CombineFinalFX->BlendMonoTech);


	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
}

void DXDeferred::RenderQuad(D3D11_VIEWPORT &vp, ID3D11ShaderResourceView* SRV, ID3DX11EffectTechnique* tech)
{
	m_DeviceContext->RSSetViewports(1, &vp);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_DeviceContext->IASetInputLayout(DXInputLayouts::Quad);

	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

	DXEffects::CombineFinalFX->SetTexture(SRV);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, m_DeviceContext);

		UINT offset = 0;
		UINT vertexStride = sizeof(DXVertex::Quad);
		m_DeviceContext->IASetVertexBuffers(0, 1, &m_FullSceenQuad, &vertexStride, &offset);
		m_DeviceContext->Draw(4, 0);
	}

	DXEffects::CombineFinalFX->SetTexture(NULL);
	tech->GetPassByIndex(0)->Apply(0, m_DeviceContext);
}

void DXDeferred::InitTestTriangle()
{
	m_TestTriangle = new DXMesh();

	std::vector<DXVertex::PosNormalTexTan> Vertices;
	std::vector<UINT> Indices;
	std::vector<DXMesh::Subset> Subsets;
	DXVertex::PosNormalTexTan a, b, c;

	//a
	a.Pos = DirectX::XMFLOAT3(-5.0f, -5.0f, 0.0f);
	a.Normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	a.Tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	a.TangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	//b
	b.Pos = DirectX::XMFLOAT3(5.0f, -5.0f, 0.0f);
	b.Normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	b.Tex = DirectX::XMFLOAT2(1.0f, 1.0f);
	b.TangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	//c
	c.Pos = DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f);
	c.Normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	c.Tex = DirectX::XMFLOAT2(0.5f, 0.0f);
	c.TangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	Vertices.push_back(a);
	Vertices.push_back(b);
	Vertices.push_back(c);

	m_TestTriangle->SetVertices(m_Device, &Vertices[0], Vertices.size());

	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(2);

	m_TestTriangle->SetIndices(m_Device, &Indices[0], Indices.size());

	DXMesh::Subset subset;
	subset.FaceCount = 1;
	subset.FaceStart = 0;
	subset.Id = 0;
	subset.VertexCount = 3;
	subset.VertexStart = 0;

	Subsets.push_back(subset);

	m_TestTriangle->SetSubsetTable(Subsets);


}


void DXDeferred::InitFullScreenQuad()
{

	std::vector<DXVertex::Quad> vertices(4);

	DXVertex::Quad v1, v2, v3, v4;

	//upper left
	v1.Pos = DirectX::XMFLOAT2(-1, 1);
	v1.Tex = DirectX::XMFLOAT2(0, 0);
	//upper right
	v2.Pos = DirectX::XMFLOAT2(1, 1);
	v2.Tex = DirectX::XMFLOAT2(1, 0);
	//lower left
	v3.Pos = DirectX::XMFLOAT2(-1, -1);
	v3.Tex = DirectX::XMFLOAT2(0, 1);
	//lower right
	v4.Pos = DirectX::XMFLOAT2(1, -1);
	v4.Tex = DirectX::XMFLOAT2(1, 1);

	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	vertices[3] = v4;

	/*
	Vertex::Quad vertices[4];
	//upper left
	vertices[0].Pos = XMFLOAT2(-1, 1);
	vertices[0].Tex = XMFLOAT2(0, 0);
	//upper right
	vertices[1].Pos = XMFLOAT2(1, 1);
	vertices[1].Tex = XMFLOAT2(1, 0);
	//lower left
	vertices[2].Pos = XMFLOAT2(-1, -1);
	vertices[2].Tex = XMFLOAT2(0, 1);
	//lower right
	vertices[3].Pos = XMFLOAT2(1, -1);
	vertices[3].Tex = XMFLOAT2(1, 1);
	*/

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(DXVertex::Quad) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];

	m_Device->CreateBuffer(&vbd, &vinitData, &m_FullSceenQuad);
}

void DXDeferred::RenderTestTriangle(ICamera* _Camera)
{
	D3D11_VIEWPORT* vp = (D3D11_VIEWPORT*)_Camera->GetViewPort();
	m_DeviceContext->RSSetViewports(1, vp);

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;

	DirectX::XMFLOAT4X4 view4x4, proj4x4;
	memcpy(&view4x4, &_Camera->GetView(), sizeof(DirectX::XMFLOAT4X4));
	memcpy(&proj4x4, &_Camera->GetProjection(), sizeof(DirectX::XMFLOAT4X4));

	view = DirectX::XMLoadFloat4x4(&view4x4);
	proj = DirectX::XMLoadFloat4x4(&proj4x4);

	ID3DX11EffectTechnique* tech;
	D3DX11_TECHNIQUE_DESC techDesc;

	//Static

	tech = DXEffects::ObjectDeferredFX->BasicTech;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		
		m_DeviceContext->RSSetState(DXRenderStates::NoCullRS);
		//m_DeviceContext->RSSetState(RenderStates::WireframeRS);
		m_DeviceContext->IASetInputLayout(DXInputLayouts::PosNormalTexTan);
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(0, 0, 0);;
		DirectX::XMMATRIX worldInvTranspose;
		DirectX::XMMATRIX worldView;
		DirectX::XMMATRIX worldInvTransposeView;
		DirectX::XMMATRIX worldViewProj;
		DirectX::XMMATRIX tex = DirectX::XMMatrixTranslation(0, 0, 0);

		//world = DirectX::XMLoadFloat4x4(&instance.GetWorld());
		//float a = MathHelper::InverseTranspose(world);

		worldView = XMMatrixMultiply(world, view);
		//worldInvTranspose = MathHelper::InverseTranspose(world);
		worldInvTranspose = world;

		//worldInvTransposeView = worldInvTranspose*view;
		worldViewProj = XMMatrixMultiply(worldView, proj);

		DXEffects::ObjectDeferredFX->SetWorld(world);
		DXEffects::ObjectDeferredFX->SetWorldInvTranspose(worldInvTranspose);
		DXEffects::ObjectDeferredFX->SetTexTransform(tex);
		DXEffects::ObjectDeferredFX->SetWorldViewProj(worldViewProj);


		tech->GetPassByIndex(p)->Apply(0, m_DeviceContext);

		m_TestTriangle->Draw(m_DeviceContext, 0);

	}

	//reset textures
	DXEffects::ObjectDeferredFX->SetDiffuseMap(NULL);
	DXEffects::ObjectDeferredFX->SetNormalMap(NULL);
}


void DXDeferred::Render(ID3D11RenderTargetView *_RenderTargetView, ICamera* _Camera)
{
	m_DeviceContext->OMSetBlendState(DXRenderStates::OpaqueBS, NULL, 0xffffffff);
	ClearBuffers();
	FillGBuffer(_Camera);
	//shadowmap->Render();
	CombineFinal(_RenderTargetView);
}