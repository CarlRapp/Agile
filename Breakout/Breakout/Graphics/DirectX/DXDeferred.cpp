#include "DXDeferred.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "DXRenderStates.h"
#include "DXEffects.h"
#include "DXVertex.h"

static const int INSTANCEDBUFFERSIZE = 200;


DXDeferred::DXDeferred(void)
{
	m_dirLightBuffer = NULL;
	m_pointLightBuffer = NULL;
	m_spotLightBuffer = NULL;

	m_dirLights = NULL;

	//DirectionalLight* sun = new DirectionalLight();
	//sun->GetGPULight()->Color = DirectX::XMFLOAT3(0.5, 0.5, 0.5);
	//sun->GetGPULight()->Direction = DirectX::XMFLOAT4(0, -3, 1, 0);
	//sun->GetGPULight()->HasShadow = false;

	//m_dirLights->push_back(sun);

	
	m_pointLights = NULL;

	//PointLight* pl = new PointLight();
	//pl->Color = &color;
	//pl->Position = &pos;
	//pl->Range = &range;
	//pl->GetGPULight()->HasShadow = false;

	//m_pointLights->push_back(pl);

	/*pl = new PointLight();
	pl->GetGPULight()->Color = DirectX::XMFLOAT3(0.0, 0, 1.0);
	pl->GetGPULight()->Position = DirectX::XMFLOAT3(5, 10, -10);
	pl->GetGPULight()->Range = 50;
	pl->GetGPULight()->HasShadow = false;*/

	//m_pointLights->push_back(pl);


	m_spotLights = NULL;

	//SpotLight* sl = new SpotLight();
	//sl->GetGPULight()->Color = DirectX::XMFLOAT3(0, 1, 0);
	//sl->GetGPULight()->Position = DirectX::XMFLOAT3(50, 10, -10);
	//sl->GetGPULight()->Direction = DirectX::XMFLOAT3(-5, 0, 1);
	//sl->GetGPULight()->angle = 50;
	//sl->GetGPULight()->Range = 50;
	//sl->GetGPULight()->HasShadow = false;

	//m_spotLights->push_back(sl);
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
	//ReleaseCOM(m_finalUAV);
	ReleaseCOM(m_fullSceenQuad);
	ReleaseCOM(m_instanceBuffer);

	/*SafeDelete(m_dirLights);
	SafeDelete(m_pointLights);
	SafeDelete(m_spotLights);*/

	SafeDelete(m_dirLightBuffer);
	SafeDelete(m_pointLightBuffer);
	SafeDelete(m_spotLightBuffer);
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
	/*D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	m_device->CreateUnorderedAccessView(finalTex, 0, &m_finalUAV);*/


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
	//m_deviceContext->ClearUnorderedAccessViewFloat(m_finalUAV, clearColor);

}

void DXDeferred::FillGBuffer(map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera)
{
	m_deviceContext->OMSetBlendState(DXRenderStates::m_opaqueBS, NULL, 0xffffffff);
	m_deviceContext->OMSetRenderTargets(2, m_GBuffer, m_depthStencilView);
	m_deviceContext->OMSetDepthStencilState(DXRenderStates::m_lessDSS, 0);
	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);

	//m_deviceContext->RSSetState(DXRenderStates::m_wireframeRS);
	//RenderTestTriangle(_camera);
	RenderModels(_modelInstances, _camera);

	m_deviceContext->OMSetRenderTargets(0, 0, 0);
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

void DXDeferred::Render2DTextures(ID3D11RenderTargetView *_renderTargetView, map<int, DX2DTextureInstance*> &_textureInstances)
{

	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	m_deviceContext->RSSetViewports(1, &m_viewPort);
	m_deviceContext->OMSetRenderTargets(1, &_renderTargetView, NULL);
	m_deviceContext->OMSetDepthStencilState(DXRenderStates::m_noDSS, 0);

	map<int, DX2DTextureInstance*>::iterator texIterator;
	for (texIterator = _textureInstances.begin(); texIterator != _textureInstances.end(); ++texIterator)
	{
		Render2DTexture(texIterator->second);
	}
}

void DXDeferred::Render2DTexture(DX2DTextureInstance *_textureInstance)
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = *_textureInstance->X * m_width;
	vp.TopLeftY = (1 - (*_textureInstance->Y + *_textureInstance->Height)) * m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.Width = *_textureInstance->Width * m_width;
	vp.Height = *_textureInstance->Height * m_height;

	RenderQuad(vp, _textureInstance->Texture, DXEffects::m_combineFinalFX->m_alphaTransparencyColorTech);
}

void DXDeferred::RenderModels(map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera)
{
	D3D11_VIEWPORT* vp = (D3D11_VIEWPORT*)_camera->GetViewPort();
	m_deviceContext->RSSetViewports(1, vp);

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;


	view = DirectX::XMLoadFloat4x4(_camera->GetView());
	proj = DirectX::XMLoadFloat4x4(_camera->GetProjection());

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
					RenderModelInstanced(&mapIterator->second, view, proj, tech, p);
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
	if(_mi->explodeTime)
		DXEffects::m_objectDeferredFX->SetExplodeTime(*_mi->explodeTime);
	else
		DXEffects::m_objectDeferredFX->SetExplodeTime(0.f);


	for (UINT subset = 0; subset < _mi->model->SubsetCount; ++subset)
	{
		//UINT subset = 6;
		DXEffects::m_objectDeferredFX->SetMaterial(_mi->model->Mat[subset]);
		//DXEffects::m_objectDeferredFX->SetMaterial(instance.GetModel()->Mat[subset]);

		if (_mi->model->HasDiffuseMaps())
			DXEffects::m_objectDeferredFX->SetDiffuseMap(_mi->model->GetDiffuseMap(subset));

		if (_mi->model->HasNormalMaps())
			DXEffects::m_objectDeferredFX->SetNormalMap(_mi->model->GetNormalMap(subset));

		_tech->GetPassByIndex(_pass)->Apply(0, m_deviceContext);
		_mi->model->ModelMesh.Draw(m_deviceContext, subset);
	}

}

void DXDeferred::RenderModelInstanced(map<int, ModelInstance*> *_mi, DirectX::CXMMATRIX _view, DirectX::CXMMATRIX _proj, ID3DX11EffectTechnique* _tech, UINT _pass)
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
		if (modelIterator->second->explodeTime)
			data.ExplodeTime = *modelIterator->second->explodeTime;
		else
		{
			data.ExplodeTime = 0;
			modelIterator->second->explodeTime = 0;
		}
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
			DXEffects::m_objectDeferredFX->SetMaterial(_mi->begin()->second->model->Mat[subset]);
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


void DXDeferred::ComputeLight(ID3D11UnorderedAccessView *_finalUAV, ICamera* _camera)
{
	DirectX::XMFLOAT4	camPosition;
	DirectX::XMFLOAT4X4 invViewProj;


	DirectX::XMFLOAT3 temp = _camera->GetPosition();
	camPosition = DirectX::XMFLOAT4(temp.x, temp.y, temp.z, 1);

	DirectX::XMMATRIX	viewMatrix;
	DirectX::XMMATRIX	projMatrix;
	DirectX::XMMATRIX	invViewProjMatrix;

	viewMatrix = DirectX::XMLoadFloat4x4(_camera->GetView());
	projMatrix = DirectX::XMLoadFloat4x4(_camera->GetProjection());

	invViewProjMatrix = XMMatrixMultiply(viewMatrix, projMatrix);

	DirectX::XMVECTOR det = XMMatrixDeterminant(invViewProjMatrix);
	invViewProjMatrix = XMMatrixInverse(&det, invViewProjMatrix);

	XMStoreFloat4x4(&invViewProj, invViewProjMatrix);


	D3DX11_TECHNIQUE_DESC	techDesc;
	ID3DX11EffectTechnique* tech;

	tech = DXEffects::m_tiledLightningFX->m_basicTech;

	tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		/*DXEffects::m_tiledLightningFX->SetViewProjTexs(&m_ViewProjTexs[0], MAX_SHADOWMAPS);
		DXEffects::m_tiledLightningFX->SetViewProj(&m_ViewProj[0], MAX_SHADOWMAPS);
		DXEffects::m_tiledLightningFX->SetTexs(&m_Texs[0], MAX_SHADOWMAPS);*/
		DXEffects::m_tiledLightningFX->SetInvViewProj(&invViewProj);
		DXEffects::m_tiledLightningFX->SetCamPosition(&camPosition);
		DXEffects::m_tiledLightningFX->SetResolution(DirectX::XMFLOAT2((float)m_width, (float)m_height));
		/*DXEffects::m_tiledLightningFX->SetShadowMapSwitches(m_ShadowMapSwitches);
		DXEffects::m_tiledLightningFX->SetShadowMapResolution(DirectX::XMFLOAT2((float)m_ShadowWidth, (float)m_ShadowHeight));*/

		DXEffects::m_tiledLightningFX->SetAlbedoMap(m_albedoSRV);
		DXEffects::m_tiledLightningFX->SetNormalSpecMap(m_normalSpecSRV);
		DXEffects::m_tiledLightningFX->SetDepthMap(m_depthSRV);
		DXEffects::m_tiledLightningFX->SetOutputMap(_finalUAV);

		//switch (m_CurrentShadowMap)
		//{
		//case 0:
		//	Effects::TiledLightningFX->SetShadowMap0(m_ShadowMapSRV0);
		//	break;
		//case 1:
		//	Effects::TiledLightningFX->SetShadowMap0(m_ShadowMapSRV0);
		//	Effects::TiledLightningFX->SetShadowMap1(m_ShadowMapSRV1);
		//	break;
		//case 2:
		//	Effects::TiledLightningFX->SetShadowMap0(m_ShadowMapSRV0);
		//	Effects::TiledLightningFX->SetShadowMap1(m_ShadowMapSRV1);
		//	Effects::TiledLightningFX->SetShadowMap2(m_ShadowMapSRV2);
		//	break;
		//case 3:
		//	Effects::TiledLightningFX->SetShadowMap0(m_ShadowMapSRV0);
		//	Effects::TiledLightningFX->SetShadowMap1(m_ShadowMapSRV1);
		//	Effects::TiledLightningFX->SetShadowMap2(m_ShadowMapSRV2);
		//	Effects::TiledLightningFX->SetShadowMap3(m_ShadowMapSRV3);
		//	break;
		//}



		ID3D11ShaderResourceView* dirLightMap = m_dirLightBuffer == NULL ? NULL : m_dirLightBuffer->GetShaderResource();
		ID3D11ShaderResourceView* pointLightMap = m_pointLightBuffer == NULL ? NULL : m_pointLightBuffer->GetShaderResource();
		ID3D11ShaderResourceView* spotLightMap = m_spotLightBuffer == NULL ? NULL : m_spotLightBuffer->GetShaderResource();

		//DXEffects::m_tiledLightningFX->SetDirLightMap(dirLightMap);
		DXEffects::m_tiledLightningFX->SetPointLightMap(pointLightMap);
		//DXEffects::m_tiledLightningFX->SetSpotLightMap(spotLightMap);
		//DXEffects::m_tiledLightningFX->SetGlobalLight(*m_GlobalLight);


		tech->GetPassByIndex(p)->Apply(0, m_deviceContext);

		// How many groups do we need to dispatch to cover a row of pixels, where each
		// group covers 256 pixels (the 256 is defined in the ComputeShader).
		UINT numGroupsX = (UINT)ceilf(m_width / 16.0f);
		UINT numGroupsY = (UINT)ceilf(m_height / 16.0f);
		m_deviceContext->Dispatch(numGroupsX, numGroupsY, 1);
	}


	// Unbind the input texture from the CS for good housekeeping.
	ID3D11ShaderResourceView* nullSRV[1] = { 0 };
	m_deviceContext->CSSetShaderResources(0, 1, nullSRV);

	// Unbind output from compute shader (we are going to use this output as an input in the next pass, 
	// and a resource cannot be both an output and input at the same time.
	ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
	m_deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);


	DXEffects::m_tiledLightningFX->SetAlbedoMap(NULL);
	DXEffects::m_tiledLightningFX->SetNormalSpecMap(NULL);
	DXEffects::m_tiledLightningFX->SetDepthMap(NULL);

	//switch (m_CurrentShadowMap)
	//{
	//case 0:
	//	Effects::TiledLightningFX->SetShadowMap0(NULL);
	//	break;
	//case 1:
	//	Effects::TiledLightningFX->SetShadowMap0(NULL);
	//	Effects::TiledLightningFX->SetShadowMap1(NULL);
	//	break;
	//case 2:
	//	Effects::TiledLightningFX->SetShadowMap0(NULL);
	//	Effects::TiledLightningFX->SetShadowMap1(NULL);
	//	Effects::TiledLightningFX->SetShadowMap2(NULL);
	//	break;
	//case 3:
	//	Effects::TiledLightningFX->SetShadowMap0(NULL);
	//	Effects::TiledLightningFX->SetShadowMap1(NULL);
	//	Effects::TiledLightningFX->SetShadowMap2(NULL);
	//	Effects::TiledLightningFX->SetShadowMap3(NULL);
	//	break;
	//}

	DXEffects::m_tiledLightningFX->SetOutputMap(NULL);

	tech->GetPassByIndex(0)->Apply(0, m_deviceContext);
	m_deviceContext->CSSetShader(0, 0, 0);
}

void DXDeferred::UpdateLights()
{
	//Update DirecionalLights.
	if (m_dirLights != NULL)
	{
		//Update size of m_DirLightBuffer
		if (m_dirLightBuffer != NULL)
		{

			if (m_dirLightBuffer->Size() != m_dirLights->size())
			{
				delete m_dirLightBuffer;

				if (m_dirLights->size() > 0)
					m_dirLightBuffer = new DXStructuredBuffer<GPUDirectionalLight>(m_device, m_dirLights->size(), D3D11_BIND_SHADER_RESOURCE, true);
				else
					m_dirLightBuffer = NULL;
			}
		}
		else if (m_dirLights->size() > 0)
			m_dirLightBuffer = new DXStructuredBuffer<GPUDirectionalLight>(m_device, m_dirLights->size(), D3D11_BIND_SHADER_RESOURCE, true);

		//Update lights in m_DirLightBuffer
		if (m_dirLightBuffer != NULL)
		{
			if (m_dirLightBuffer->Size() > 0)
			{
				GPUDirectionalLight* dirLight = m_dirLightBuffer->MapDiscard(m_deviceContext);
				for (unsigned int i = 0; i < m_dirLights->size(); ++i)
				{
					dirLight[i] = *m_dirLights->at(i)->GetGPULight();
				}
				m_dirLightBuffer->Unmap(m_deviceContext);
			}
		}
	}
	else if (m_dirLightBuffer != NULL)
	{
		delete m_dirLightBuffer;
		m_dirLightBuffer = NULL;
	}

	//Update PointLights.
	if (m_pointLights != NULL)
	{
		//Update size of m_PointLightBuffer
		if (m_pointLightBuffer != NULL)
		{
			if (m_pointLightBuffer->Size() != m_pointLights->size())
			{
				delete m_pointLightBuffer;

				if (m_pointLights->size() > 0)
					m_pointLightBuffer = new DXStructuredBuffer<GPUPointLight>(m_device, m_pointLights->size(), D3D11_BIND_SHADER_RESOURCE, true);
				else
					m_pointLightBuffer = NULL;
			}
		}
		else if (m_pointLights->size() > 0)
			m_pointLightBuffer = new DXStructuredBuffer<GPUPointLight>(m_device, m_pointLights->size(), D3D11_BIND_SHADER_RESOURCE, true);


		//Update lights in m_PointLightBuffer
		if (m_pointLightBuffer != NULL)
		{
			if (m_pointLightBuffer->Size() > 0)
			{
				GPUPointLight* pointLight = m_pointLightBuffer->MapDiscard(m_deviceContext);

				map<int, PointLight*>::iterator lightIterator;
				int i = 0;
				for (lightIterator = m_pointLights->begin(); lightIterator != m_pointLights->end(); ++lightIterator)
				{
					pointLight[i] = *lightIterator->second->GetGPULight();
					i++;
				}
				m_pointLightBuffer->Unmap(m_deviceContext);
			}
		}
	}
	else if (m_pointLightBuffer != NULL)
	{
		delete m_pointLightBuffer;
		m_pointLightBuffer = NULL;
	}

	//Update SpotLights.
	if (m_spotLights != NULL)
	{
		//Update size of m_SpotLightBuffer
		if (m_spotLightBuffer != NULL)
		{
			if (m_spotLightBuffer->Size() != m_spotLights->size())
			{
				delete m_spotLightBuffer;

				if (m_spotLights->size() > 0)
					m_spotLightBuffer = new DXStructuredBuffer<GPUSpotLight>(m_device, m_spotLights->size(), D3D11_BIND_SHADER_RESOURCE, true);
				else
					m_spotLightBuffer = NULL;
			}
		}
		else if (m_spotLights->size() > 0)
			m_spotLightBuffer = new DXStructuredBuffer<GPUSpotLight>(m_device, m_spotLights->size(), D3D11_BIND_SHADER_RESOURCE, true);

		//Update lights in m_SpotLightBuffer
		if (m_spotLightBuffer != NULL)
		{
			if (m_spotLightBuffer->Size() > 0)
			{
				GPUSpotLight* spotLight = m_spotLightBuffer->MapDiscard(m_deviceContext);
				for (unsigned int i = 0; i < m_spotLights->size(); ++i)
				{
					spotLight[i] = *m_spotLights->at(i)->GetGPULight();
				}
				m_spotLightBuffer->Unmap(m_deviceContext);
			}
		}
	}
	else if (m_spotLightBuffer != NULL)
	{
		delete m_spotLightBuffer;
		m_spotLightBuffer = NULL;
	}
}


float ddda = 0.0f;
void DXDeferred::Render(float _dt,
	ID3D11RenderTargetView *_renderTargetView,
	ID3D11UnorderedAccessView *_finalUAV,
	map<std::string, map<int, ModelInstance*>> &_modelInstances, 
	map<int, DX2DTextureInstance*> &_textureInstances,
	map<int, DXParticleSystem*>		&_particleSystems,
	ICamera* _camera)
{

	ddda += 0.004;
	//pos.x = 15 + 10 * sinf(ddda);
	//pos.z = 10 * cosf(ddda);

	
	ClearBuffers();


	UpdateLights();
	FillGBuffer(_modelInstances, _camera);
	ComputeLight(_finalUAV, _camera);

	RenderParticleSystems(_dt, _renderTargetView, _particleSystems, _camera);

	Render2DTextures(_renderTargetView, _textureInstances);




	//shadowmap->Render();
	//CombineFinal(_renderTargetView);
}

void DXDeferred::RenderParticleSystems(float _dt, ID3D11RenderTargetView *_renderTargetView, map<int, DXParticleSystem*> &_particleSystems, ICamera* _camera)
{
	m_deviceContext->OMSetDepthStencilState(DXRenderStates::m_noDSS, 0);
	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	m_deviceContext->RSSetViewports(1, &m_viewPort);
	m_deviceContext->OMSetRenderTargets(1, &_renderTargetView, m_depthStencilView);
	//m_deviceContext->OMSetRenderTargets(1, &_renderTargetView, NULL);

	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(_camera->GetView());
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(_camera->GetProjection());	

	DirectX::XMFLOAT4X4 viewProj;
	DirectX::XMStoreFloat4x4(&viewProj, XMMatrixMultiply(view, proj));

	map<int, DXParticleSystem*>::iterator psIterator;
	for (psIterator = _particleSystems.begin(); psIterator != _particleSystems.end(); ++psIterator)
	{
		psIterator->second->Render(m_deviceContext, _dt, viewProj, _camera->GetPosition());
	}
}

void DXDeferred::CombineFinal(ID3D11RenderTargetView *_renderTargetView)
{
	

	m_deviceContext->RSSetState(DXRenderStates::m_noCullRS);
	m_deviceContext->RSSetViewports(1, &m_viewPort);
	m_deviceContext->OMSetRenderTargets(1, &_renderTargetView, NULL);
	m_deviceContext->OMSetDepthStencilState(DXRenderStates::m_noDSS, 0);

	//DXEffects::CombineFinalFX->SetTexture(m_FinalSRV);
	//Effects::CombineFinalFX->SetTexture(m_ShadowMapSRV0);


	RenderQuad(m_viewPort, m_finalSRV, DXEffects::m_combineFinalFX->m_colorTech);
	//RenderQuad(m_viewPort, m_albedoSRV, DXEffects::m_combineFinalFX->m_colorTech);
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