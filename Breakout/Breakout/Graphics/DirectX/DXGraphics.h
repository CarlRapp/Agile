#pragma once
#ifndef DXGRAPHICS_H
#define DXGRAPHICS_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

#include "DXModelManager.h"
#include "DXTextureManager.h"
#include "DX2DTextureInstance.h"
#include "DXParticleSystem.h"

#include "../IGraphics.h"
#include "DXWindow.h"
#include "DXDeferred.h"
#include "../ICamera.h"
#include <map>




class DXGraphics : public IGraphics
{

private:
	DXWindow				*m_window;

	DXTextureManager		m_textureManager;

	//map<int, ModelInstance*>m_modelInstances;


	map<std::string, map<int, ModelInstance*>>	m_modelInstances;
	map<int, DX2DTextureInstance*>	m_textureInstances;

	map<int, DirectionalLight*>			m_dirLights;
	map<int, PointLight*>				m_pointLights;
	map<int, SpotLight*>				m_spotLights;
	map<int, DXParticleSystem*>			m_particleSystems;



	DXModelManager			m_modelManager;
	DXModel					*m_testmodel;
	ModelInstance			*m_testmodelinstance;

	IDXGISwapChain				*m_swapChain = NULL;
	ID3D11RenderTargetView		*m_renderTargetView = NULL;
	ID3D11UnorderedAccessView	*m_finalUAV;

	ID3D11Texture2D			*m_depthStencil = NULL;
	ID3D11DepthStencilView	*m_depthStencilView = NULL;
	ID3D11Device			*m_device = NULL;
	ID3D11DeviceContext		*m_deviceContext = NULL;

	DXDeferred				*m_DXDeferred = NULL;


	HRESULT					InitDirect3D(DisplayMode _displayMode);

	int m_width, m_height;



public:

	DXGraphics(void);
	~DXGraphics(void);

	bool  InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode);
	bool  Init3D(DisplayMode _displayMode);
	DXWindow* GetWindow(){ return m_window; }
	void	Update();
	void	Render(float _dt, ICamera* _camera);
	void LoadModel(std::string _path);
	void LoadTexture(std::string _path);

	void AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose, float* _explosion);
	void RemoveObject(int _id);

	void Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height);
	void Remove2DTexture(int _id);

	void AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range);
	void RemovePointLight(int _id);

	void AddEffect(int _id, std::string _effect, VECTOR3 *_pos, VECTOR3 *_vel);
	void RemoveEffect(int _id);

};



#endif