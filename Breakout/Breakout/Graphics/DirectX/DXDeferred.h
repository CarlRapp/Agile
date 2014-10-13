#pragma once
#ifndef DXDEFERRED_H
#define DXDEFERRED_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>
#include "DXText.h"

#include "../IGraphics.h"
#include "DXWindow.h"
#include "../ICamera.h"
#include "DXMesh.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "DXRenderStates.h"
#include "DXEffects.h"
#include "DXVertex.h"
#include "DXModel.h"
#include "DX2DTextureInstance.h"
#include "DXStructuredBuffer.h"
#include "DXLightHelper.h"
#include "DXParticleSystem.h"

class DXDeferred
{

private:

	ID3D11Buffer			*m_instanceBuffer;

	//ID3D11Texture2D			*m_depthStencil = NULL;
	//ID3D11DepthStencilView	*m_depthStencilView = NULL;
	ID3D11Device			*m_device = NULL;
	ID3D11DeviceContext		*m_deviceContext = NULL;

	D3D11_VIEWPORT			m_viewPort;

	int						m_width, m_height;

	ID3D11DepthStencilView		*m_depthStencilView, *m_shadowMapDSV;
	ID3D11RenderTargetView		*m_albedoRTV, *m_normalSpecRTV;
	ID3D11ShaderResourceView	*m_albedoSRV, *m_normalSpecSRV, *m_depthSRV, *m_finalSRV;

	//ID3D11UnorderedAccessView	*m_finalUAV;

	ID3D11RenderTargetView* m_GBuffer[2];

	
	map<int, DirectionalLight*>				*m_dirLights;
	map<int, PointLight*>					*m_pointLights;
	map<int, SpotLight*>					*m_spotLights;

	DXStructuredBuffer<GPUDirectionalLight>	*m_dirLightBuffer;
	DXStructuredBuffer<GPUPointLight>			*m_pointLightBuffer;
	DXStructuredBuffer<GPUSpotLight>			*m_spotLightBuffer;

	//TEST
	void					RenderQuad(D3D11_VIEWPORT &_vp, ID3D11ShaderResourceView* _SRV, ID3DX11EffectTechnique* _tech);
	ID3D11Buffer			*m_fullSceenQuad;
	void					InitFullScreenQuad();


	void					InitBuffers();

	void					ClearBuffers();
	void					FillGBuffer(map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera);
	void					CombineFinal(ID3D11RenderTargetView *_renderTargetView);

	void					RenderModels(map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera);
	void					RenderModel(ModelInstance* _mi, DirectX::CXMMATRIX _view, DirectX::CXMMATRIX _proj, ID3DX11EffectTechnique* _tech, UINT _pass);
	void					RenderModelInstanced(map<int, ModelInstance*> *_mi, DirectX::CXMMATRIX _view, DirectX::CXMMATRIX _proj, ID3DX11EffectTechnique* _tech, UINT _pass);

	void					Render2DTextures(ID3D11RenderTargetView *_renderTargetView, map<int, DX2DTextureInstance*> &_textureInstances);
	void					Render2DTexture(DX2DTextureInstance *_textureInstance);

	void					RenderParticleSystems(float _dt, ID3D11RenderTargetView *_renderTargetView, map<int, DXParticleSystem*> &_particleSystems, ICamera* _camera);

	void					ComputeLight(ID3D11UnorderedAccessView *_renderTargetView, ICamera* _camera);

	void					UpdateLights();

	void					RenderTexts(ID3D11RenderTargetView *_renderTargetView, map<int, DXText::String*> &_texts, ID3D11ShaderResourceView* _symbolsTex, int _numSymbols);
	void					RenderText(ID3D11RenderTargetView *_renderTargetView, DXText::String *_text);

public:

	DXDeferred(void);
	~DXDeferred(void);

	void Init(ID3D11Device *_device, ID3D11DeviceContext *_deviceContext, int _width, int _height);

	void	Render(float _dt,
		ID3D11RenderTargetView *_renderTargetView,
		ID3D11UnorderedAccessView *_finalUAV,
		map<std::string, map<int, ModelInstance*>> &_modelInstances, 
		map<int, DX2DTextureInstance*> &_textureInstances,
		map<int, DXParticleSystem*>		&_particleSystems,
		map<int, DXText::String*>		&_texts,
		ID3D11ShaderResourceView* _symbolsTex,
		int _numSymbols,
		ICamera* _camera);

	void SetDirectionLightMap(map<int, DirectionalLight*> *map) { m_dirLights = map; }
	void SetPointLightMap(map<int, PointLight*> *map) { m_pointLights = map; }
	void SetSpotLightMap(map<int, SpotLight*> *map) { m_spotLights = map; }

};



#endif