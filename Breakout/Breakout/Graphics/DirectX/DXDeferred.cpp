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
	ReleaseCOM(m_depthStencilView);
	ReleaseCOM(m_shadowMapDSV);
	ReleaseCOM(m_albedoRTV);
	ReleaseCOM(m_normalSpecRTV);
	ReleaseCOM(m_albedoSRV);
	ReleaseCOM(m_normalSpecSRV);
	ReleaseCOM(m_depthSRV);
	ReleaseCOM(m_finalSRV);
	ReleaseCOM(m_finalUAV);
	ReleaseCOM(m_fullSceenQuad);
	SafeDelete(m_testTriangle);
}

void DXDeferred::Init(ID3D11Device *_Device, ID3D11DeviceContext *_DeviceContext, int _Width, int _Height)
{
	m_device = _Device;
	m_deviceContext = _DeviceContext;

	m_width = _Width;
	m_height = _Height;


	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = (float)0;
	m_viewPort.TopLeftY = (float)0;
	m_viewPort.Width = (float)m_width;
	m_viewPort.Height = (float)m_height;

	InitTestTriangle();
	InitFullScreenQuad();
	InitBuffers();

}

void DXDeferred::InitBuffers()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = m_width;
	texDesc.Height = m_height;
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
	m_device->CreateTexture2D(&texDesc, NULL, &normalSpecTex);

	//R8G8B8A8_UNORM
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_device->CreateTexture2D(&texDesc, NULL, &albedoTex);

	//D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_device->CreateTexture2D(&texDesc, NULL, &finalTex);

	//R24G8_TYPELESS & D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	m_device->CreateTexture2D(&texDesc, NULL, &depthTex);


	//RENDER TARGETS
	m_device->CreateRenderTargetView(albedoTex, NULL, &m_albedoRTV);
	m_device->CreateRenderTargetView(normalSpecTex, NULL, &m_normalSpecRTV);

	//GBUFFER
	m_GBuffer[0] = m_albedoRTV;
	m_GBuffer[1] = m_normalSpecRTV;

	//SHADERRESOURCEVIEWS
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	//R8G8B8A8_UNORM
	m_device->CreateShaderResourceView(albedoTex, &SRVDesc, &m_albedoSRV);
	m_device->CreateShaderResourceView(finalTex, 0, &m_finalSRV);

	//DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	m_device->CreateShaderResourceView(depthTex, &SRVDesc, &m_depthSRV);

	//DXGI_FORMAT_R16G16B16A16_UNORM
	SRVDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	m_device->CreateShaderResourceView(normalSpecTex, 0, &m_normalSpecSRV);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(depthTex, &descDSV, &m_depthStencilView);

	//Create the UnorderedAccessView
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	m_device->CreateUnorderedAccessView(finalTex, 0, &m_finalUAV);


	//RELEASE
	albedoTex->Release();
	normalSpecTex->Release();
	depthTex->Release();
	finalTex->Release();
}

void DXDeferred::ClearBuffers()
{
	m_deviceContext->RSSetViewports(1, &m_viewPort);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	float albedoClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//m_deviceContext->ClearRenderTargetView( m_RenderTargetView, ClearColor );
	m_deviceContext->ClearRenderTargetView(m_albedoRTV, albedoClearColor);
	m_deviceContext->ClearRenderTargetView(m_normalSpecRTV, clearColor);
	m_deviceContext->ClearUnorderedAccessViewFloat(m_finalUAV, clearColor);

}

void DXDeferred::FillGBuffer(ICamera* _camera)
{
	m_deviceContext->OMSetRenderTargets(2, m_GBuffer, m_depthStencilView);


	m_deviceContext->OMSetDepthStencilState(DXRenderStates::m_lessDSS, 0);

	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	//m_deviceContext->RSSetState(DXRenderStates::m_wireframeRS);
	RenderTestTriangle(_camera);

	m_deviceContext->OMSetRenderTargets(0, 0, 0);
}

void DXDeferred::CombineFinal(ID3D11RenderTargetView *_renderTargetView)
{
	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	m_deviceContext->RSSetViewports(1, &m_viewPort);
	m_deviceContext->OMSetRenderTargets(1, &_renderTargetView, NULL);
	m_deviceContext->OMSetDepthStencilState(DXRenderStates::m_noDSS, 0);

	//DXEffects::CombineFinalFX->SetTexture(m_FinalSRV);
	//Effects::CombineFinalFX->SetTexture(m_ShadowMapSRV0);


	RenderQuad(m_viewPort, m_albedoSRV, DXEffects::m_combineFinalFX->m_colorTech);
	//RenderQuad(m_ViewPort, m_NormalSpecSRV, Effects::CombineFinalFX->ColorTech);
	//Effects::CombineFinalFX->SetOpacity(0.8f);
	//RenderQuad(shadowVP, m_ShadowMapSRV, Effects::CombineFinalFX->BlendMonoTech);


	m_deviceContext->RSSetViewports(1, &m_viewPort);
}

void DXDeferred::RenderQuad(D3D11_VIEWPORT &_vp, ID3D11ShaderResourceView* _SRV, ID3DX11EffectTechnique* _tech)
{
	m_deviceContext->RSSetViewports(1, &_vp);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_deviceContext->IASetInputLayout(DXInputLayouts::m_quad);

	D3DX11_TECHNIQUE_DESC techDesc;
	_tech->GetDesc(&techDesc);

	DXEffects::m_combineFinalFX->SetTexture(_SRV);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		_tech->GetPassByIndex(p)->Apply(0, m_deviceContext);

		UINT offset = 0;
		UINT vertexStride = sizeof(DXVertex::Quad);
		m_deviceContext->IASetVertexBuffers(0, 1, &m_fullSceenQuad, &vertexStride, &offset);
		m_deviceContext->Draw(4, 0);
	}

	DXEffects::m_combineFinalFX->SetTexture(NULL);
	_tech->GetPassByIndex(0)->Apply(0, m_deviceContext);
}

void DXDeferred::InitTestTriangle()
{
	m_testTriangle = new DXMesh();

	std::vector<DXVertex::PosNormalTexTanCol> Vertices;
	std::vector<UINT> Indices;
	std::vector<DXMesh::Subset> Subsets;
	DXVertex::PosNormalTexTanCol a, b, c, d, e, f;

	//a
	a.pos = DirectX::XMFLOAT3(-5.0f, -5.0f, -5.0f);
	a.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	a.tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	a.tangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	a.color = DirectX::XMFLOAT3(0.8f, 0.0f, 1.0f);

	//b
	b.pos = DirectX::XMFLOAT3(5.0f, -5.0f, -5.0f);
	b.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	b.tex = DirectX::XMFLOAT2(1.0f, 1.0f);
	b.tangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	b.color = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

	//c
	c.pos = DirectX::XMFLOAT3(0.0f, 5.0f, -5.0f);
	c.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	c.tex = DirectX::XMFLOAT2(0.5f, 0.0f);
	c.tangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	c.color = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);


	//d
	d.pos = DirectX::XMFLOAT3(-5.0f, -5.0f, 5.0f);
	d.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	d.tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	d.tangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	d.color = DirectX::XMFLOAT3(0.8f, 0.0f, 1.0f);

	//e
	e.pos = DirectX::XMFLOAT3(5.0f, -5.0f, 5.0f);
	e.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	e.tex = DirectX::XMFLOAT2(1.0f, 1.0f);
	e.tangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	e.color = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

	//f
	f.pos = DirectX::XMFLOAT3(0.0f, 5.0f, 5.0f);
	f.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	f.tex = DirectX::XMFLOAT2(0.5f, 0.0f);
	f.tangentU = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	f.color = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);


	Vertices.push_back(a);
	Vertices.push_back(b);
	Vertices.push_back(c);

	Vertices.push_back(d);
	Vertices.push_back(e);
	Vertices.push_back(f);

	m_testTriangle->SetVertices(m_device, &Vertices[0], Vertices.size());

	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(2);

	Indices.push_back(3);
	Indices.push_back(4);
	Indices.push_back(5);

	m_testTriangle->SetIndices(m_device, &Indices[0], Indices.size());

	DXMesh::Subset subset;
	subset.m_faceCount = 2;
	subset.m_faceStart = 0;
	subset.m_id = 0;
	subset.m_vertexCount = 6;
	subset.m_vertexStart = 0;

	Subsets.push_back(subset);

	m_testTriangle->SetSubsetTable(Subsets);


}


void DXDeferred::InitFullScreenQuad()
{

	std::vector<DXVertex::Quad> vertices(4);

	DXVertex::Quad v1, v2, v3, v4;

	//upper left
	v1.pos = DirectX::XMFLOAT2(-1, 1);
	v1.tex = DirectX::XMFLOAT2(0, 0);
	//upper right
	v2.pos = DirectX::XMFLOAT2(1, 1);
	v2.tex = DirectX::XMFLOAT2(1, 0);
	//lower left
	v3.pos = DirectX::XMFLOAT2(-1, -1);
	v3.tex = DirectX::XMFLOAT2(0, 1);
	//lower right
	v4.pos = DirectX::XMFLOAT2(1, -1);
	v4.tex = DirectX::XMFLOAT2(1, 1);

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

	m_device->CreateBuffer(&vbd, &vinitData, &m_fullSceenQuad);
}

void DXDeferred::RenderTestTriangle(ICamera* _camera)
{
	D3D11_VIEWPORT* vp = (D3D11_VIEWPORT*)_camera->GetViewPort();
	m_deviceContext->RSSetViewports(1, vp);

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;

	DirectX::XMFLOAT4X4 view4x4, proj4x4;
	memcpy(&view4x4, &_camera->GetView(), sizeof(DirectX::XMFLOAT4X4));
	memcpy(&proj4x4, &_camera->GetProjection(), sizeof(DirectX::XMFLOAT4X4));

	view = DirectX::XMLoadFloat4x4(&view4x4);
	proj = DirectX::XMLoadFloat4x4(&proj4x4);

	ID3DX11EffectTechnique* tech;
	D3DX11_TECHNIQUE_DESC techDesc;

	//Static

	tech = DXEffects::m_objectDeferredFX->m_basicTech;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		
		m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
		//m_DeviceContext->RSSetState(RenderStates::m_wireframeRS);
		m_deviceContext->IASetInputLayout(DXInputLayouts::m_posNormalTexTanCol);
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

		DXEffects::m_objectDeferredFX->SetWorld(world);
		DXEffects::m_objectDeferredFX->SetWorldInvTranspose(worldInvTranspose);
		DXEffects::m_objectDeferredFX->SetTexTransform(tex);
		DXEffects::m_objectDeferredFX->SetWorldViewProj(worldViewProj);


		tech->GetPassByIndex(p)->Apply(0, m_deviceContext);

		m_testTriangle->Draw(m_deviceContext, 0);

	}

	//reset textures
	DXEffects::m_objectDeferredFX->SetDiffuseMap(NULL);
	DXEffects::m_objectDeferredFX->SetNormalMap(NULL);
}


void DXDeferred::Render(ID3D11RenderTargetView *_renderTargetView, ICamera* _camera)
{
	m_deviceContext->OMSetBlendState(DXRenderStates::m_opaqueBS, NULL, 0xffffffff);
	ClearBuffers();
	FillGBuffer(_camera);
	//shadowmap->Render();
	CombineFinal(_renderTargetView);
}