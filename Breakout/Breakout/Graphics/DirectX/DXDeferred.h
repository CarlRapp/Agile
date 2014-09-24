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
#include "DXModel.h"
#include "DXStructuredBuffer.h"
#include "DXLightHelper.h"

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

	
	vector<DirectionalLight*>			*m_dirLights;
	vector<PointLight*>					*m_pointLights;
	vector<SpotLight*>					*m_spotLights;

	DXStructuredBuffer<GPUDirectionalLight>	*m_dirLightBuffer;
	DXStructuredBuffer<GPUPointLight>			*m_pointLightBuffer;
	DXStructuredBuffer<GPUSpotLight>			*m_spotLightBuffer;

	//TEST
	void					RenderQuad(D3D11_VIEWPORT &_vp, ID3D11ShaderResourceView* _SRV, ID3DX11EffectTechnique* _tech);
	ID3D11Buffer			*m_fullSceenQuad;
	void					InitFullScreenQuad();


	void					InitBuffers();

	void					ClearBuffers();
	void					FillGBuffer(ID3D11Device *_device, map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera);
	void					CombineFinal(ID3D11RenderTargetView *_renderTargetView);

	void					RenderModels(ID3D11Device *_device, map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera);
	void					RenderModel(ModelInstance* _mi, DirectX::CXMMATRIX _view, DirectX::CXMMATRIX _proj, ID3DX11EffectTechnique* _tech, UINT _pass);
	void					RenderModelInstanced(ID3D11Device *_device, map<int, ModelInstance*> *_mi, DirectX::CXMMATRIX _view, DirectX::CXMMATRIX _proj, ID3DX11EffectTechnique* _tech, UINT _pass);

	void					ComputeLight(ID3D11UnorderedAccessView *_renderTargetView, ICamera* _camera);

	void					UpdateLights();

public:

	DXDeferred(void);
	~DXDeferred(void);

	void Init(ID3D11Device *_device, ID3D11DeviceContext *_deviceContext, int _width, int _height);

	void	Render(ID3D11Device *_device, ID3D11UnorderedAccessView *_renderTargetView, map<std::string, map<int, ModelInstance*>> &_modelInstances, ICamera* _camera);

};



#endif