#include "DXDeferred.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "DXRenderStates.h"
#include "DXEffects.h"
#include "DXVertex.h"

static const int INSTANCEDBUFFERSIZE = 200;

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
	ReleaseCOM(m_instanceBuffer);
}

void DXDeferred::Init(ID3D11Device *_device, ID3D11DeviceContext *_deviceContext, int _width, int _height)
{
	m_device = _device;
	m_deviceContext = _deviceContext;

	m_width = _width;
	m_height = _height;


	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = (float)0;
	m_viewPort.TopLeftY = (float)0;
	m_viewPort.Width = (float)m_width;
	m_viewPort.Height = (float)m_height;

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


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(DXInstance::World) * INSTANCEDBUFFERSIZE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	m_device->CreateBuffer(&vbd, 0, &m_instanceBuffer);

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

void DXDeferred::FillGBuffer(ID3D11Device *_device, map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera)
{
	m_deviceContext->OMSetRenderTargets(2, m_GBuffer, m_depthStencilView);


	m_deviceContext->OMSetDepthStencilState(DXRenderStates::m_lessDSS, 0);

	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	//m_deviceContext->RSSetState(DXRenderStates::m_wireframeRS);
	//RenderTestTriangle(_camera);
	RenderModels(_device, _modelInstances, _camera);

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
	//RenderQuad(m_viewPort, m_normalSpecSRV, DXEffects::m_combineFinalFX->m_colorTech);
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

void DXDeferred::RenderModels(ID3D11Device *_device, map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera)
{
	D3D11_VIEWPORT* vp = (D3D11_VIEWPORT*)_camera->GetViewPort();
	m_deviceContext->RSSetViewports(1, vp);

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;

	DirectX::XMFLOAT4X4 view4x4, proj4x4;
	memcpy(&view4x4, _camera->GetView(), sizeof(DirectX::XMFLOAT4X4));
	memcpy(&proj4x4, _camera->GetProjection(), sizeof(DirectX::XMFLOAT4X4));

	view = DirectX::XMLoadFloat4x4(&view4x4);
	proj = DirectX::XMLoadFloat4x4(&proj4x4);

	ID3DX11EffectTechnique* tech;
	D3DX11_TECHNIQUE_DESC techDesc;

	map<std::string, map<int, ModelInstance*>>::iterator	mapIterator;
	for (mapIterator = _modelInstances.begin(); mapIterator != _modelInstances.end(); ++mapIterator)
	{
		if (!mapIterator->second.empty())
		{
			//Normal
			if (mapIterator->second.size() < 5)
			{
				tech = DXEffects::m_objectDeferredFX->m_basicTech;
				if (mapIterator->second.begin()->second->model->HasDiffuseMaps())
				{
					tech = DXEffects::m_objectDeferredFX->m_texTech;
					if (mapIterator->second.begin()->second->model->HasNormalMaps())
						tech = DXEffects::m_objectDeferredFX->m_texNormalTech;
				}
				else if (mapIterator->second.begin()->second->model->HasNormalMaps())
					tech = DXEffects::m_objectDeferredFX->m_normalTech;
				tech->GetDesc(&techDesc);

				for (UINT p = 0; p < techDesc.Passes; ++p)
				{
					map<int, ModelInstance*>::iterator	modelIterator;
					for (modelIterator = mapIterator->second.begin(); modelIterator != mapIterator->second.end(); ++modelIterator)
					{
						RenderModel(modelIterator->second, view, proj, tech, p);
					}
				}
			}

			//Instanced
			else
			{
				tech = DXEffects::m_objectDeferredFX->m_basicInstancedTech;
				if (mapIterator->second.begin()->second->model->HasDiffuseMaps())
				{
					tech = DXEffects::m_objectDeferredFX->m_texInstancedTech;
					if (mapIterator->second.begin()->second->model->HasNormalMaps())
						tech = DXEffects::m_objectDeferredFX->m_texNormalInstancedTech;
				}
				else if (mapIterator->second.begin()->second->model->HasNormalMaps())
					tech = DXEffects::m_objectDeferredFX->m_normalInstancedTech;
				tech->GetDesc(&techDesc);

				for (UINT p = 0; p < techDesc.Passes; ++p)
				{
					RenderModelInstanced(_device, &mapIterator->second, view, proj, tech, p);
				}
			}
		}
	}

	//reset textures
	DXEffects::m_objectDeferredFX->SetDiffuseMap(NULL);
	DXEffects::m_objectDeferredFX->SetNormalMap(NULL);
}

void DXDeferred::RenderModel(ModelInstance* _mi, DirectX::CXMMATRIX _view, DirectX::CXMMATRIX _proj, ID3DX11EffectTechnique* _tech, UINT _pass)
{

	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	//m_DeviceContext->RSSetState(RenderStates::m_wireframeRS);
	m_deviceContext->IASetInputLayout(DXInputLayouts::m_posNormalTexTan);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(_mi->world);
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX worldView;
	DirectX::XMMATRIX worldInvTransposeView;
	DirectX::XMMATRIX worldViewProj;
	DirectX::XMMATRIX tex = DirectX::XMMatrixTranslation(0, 0, 0);

	//world = DirectX::XMLoadFloat4x4(&instance.GetWorld());
	//float a = MathHelper::InverseTranspose(world);

	worldView = XMMatrixMultiply(world, _view);
	//worldInvTranspose = MathHelper::InverseTranspose(world);
	worldInvTranspose = world;

	//worldInvTransposeView = worldInvTranspose*view;
	worldViewProj = XMMatrixMultiply(worldView, _proj);

	DXEffects::m_objectDeferredFX->SetWorld(world);
	DXEffects::m_objectDeferredFX->SetWorldInvTranspose(worldInvTranspose);
	DXEffects::m_objectDeferredFX->SetTexTransform(tex);
	DXEffects::m_objectDeferredFX->SetWorldViewProj(worldViewProj);


	for (UINT subset = 0; subset < _mi->model->SubsetCount; ++subset)
	{
		//UINT subset = 6;
		//DXEffects::m_objectDeferredFX->SetMaterial(instance.GetModel()->Mat[subset]);

		if (_mi->model->HasDiffuseMaps())
			DXEffects::m_objectDeferredFX->SetDiffuseMap(_mi->model->GetDiffuseMap(subset));

		if (_mi->model->HasNormalMaps())
			DXEffects::m_objectDeferredFX->SetNormalMap(_mi->model->GetNormalMap(subset));

		_tech->GetPassByIndex(_pass)->Apply(0, m_deviceContext);
		_mi->model->ModelMesh.Draw(m_deviceContext, subset);
	}

}

void DXDeferred::RenderModelInstanced(ID3D11Device *_device, map<int, ModelInstance*> *_mi, DirectX::CXMMATRIX _view, DirectX::CXMMATRIX _proj, ID3DX11EffectTechnique* _tech, UINT _pass)
{
	if (_mi->empty())
		return;

	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	//m_DeviceContext->RSSetState(RenderStates::m_wireframeRS);
	m_deviceContext->IASetInputLayout(DXInputLayouts::m_instancedPosNormalTexTan);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	

	DirectX::XMMATRIX viewProj;
	DirectX::XMMATRIX tex = DirectX::XMMatrixTranslation(0, 0, 0);


	//worldInvTransposeView = worldInvTranspose*view;
	viewProj = XMMatrixMultiply(_view, _proj);

	DXEffects::m_objectDeferredFX->SetTexTransform(tex);
	DXEffects::m_objectDeferredFX->SetViewProj(viewProj);

	vector<DXInstance::World> instancedData;


	map<int, ModelInstance*>::iterator	modelIterator;
	for (modelIterator = _mi->begin(); modelIterator != _mi->end(); ++modelIterator)
	{
		DXInstance::World data;
		data.world = *modelIterator->second->world;
		instancedData.push_back(data);
	}


	D3D11_MAPPED_SUBRESOURCE mappedData;


	for (int k = 0; k < instancedData.size();)
	{
		m_deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

		DXInstance::World* dataView = reinterpret_cast<DXInstance::World*>(mappedData.pData);
		int i;
		for (i = 0; k < instancedData.size() && i < INSTANCEDBUFFERSIZE; ++i)
		{
			dataView[i] = instancedData[k];
			++k;
		}

		m_deviceContext->Unmap(m_instanceBuffer, 0);

		for (UINT subset = 0; subset < _mi->begin()->second->model->SubsetCount; ++subset)
		{
			//UINT subset = 6;
			//DXEffects::m_objectDeferredFX->SetMaterial(instance.GetModel()->Mat[subset]);

			if (_mi->begin()->second->model->HasDiffuseMaps())
				DXEffects::m_objectDeferredFX->SetDiffuseMap(_mi->begin()->second->model->GetDiffuseMap(subset));

			if (_mi->begin()->second->model->HasNormalMaps())
				DXEffects::m_objectDeferredFX->SetNormalMap(_mi->begin()->second->model->GetNormalMap(subset));

			_tech->GetPassByIndex(_pass)->Apply(0, m_deviceContext);
			_mi->begin()->second->model->ModelMesh.DrawInstanced(m_deviceContext, subset, m_instanceBuffer, i);
		}
	}	
	


}


void DXDeferred::Render(ID3D11Device *_device, ID3D11RenderTargetView *_renderTargetView, map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera)
{
	m_deviceContext->OMSetBlendState(DXRenderStates::m_opaqueBS, NULL, 0xffffffff);
	ClearBuffers();
	FillGBuffer(_device, _modelInstances, _camera);
	//shadowmap->Render();
	CombineFinal(_renderTargetView);
}