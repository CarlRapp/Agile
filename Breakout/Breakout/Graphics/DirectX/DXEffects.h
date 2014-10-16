#pragma once
#ifndef DXEFFECTS_H
#define DXEFFECTS_H

#include "../../stdafx.h"

#include "DXLightHelper.h"

#include <D3D11.h>
#include <d3dCompiler.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <DirectXMath.h>

#include <d3dx11effect.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "Effects11D.lib")
#else
#pragma comment(lib, "Effects11.lib")
#endif


#pragma region DXEffect
class DXEffect
{
public:
	DXEffect(ID3D11Device* _device, const std::wstring& _filename);
	virtual ~DXEffect();

private:
	DXEffect(const DXEffect& _rhs);
	DXEffect& operator=(const DXEffect& _rhs);

protected:
	ID3DX11Effect* m_FX;
};
#pragma endregion




#pragma region ObjectDeferredEffect
class ObjectDeferredEffect : public DXEffect
{
public:
	ObjectDeferredEffect(ID3D11Device* _device, const std::wstring& _filename);
	~ObjectDeferredEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX _M)					{ m_worldViewProj->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetViewProj(DirectX::CXMMATRIX _M)							{ m_viewProj->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetWorld(DirectX::CXMMATRIX _M)							{ m_world->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX _M)				{ m_worldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetTexTransform(DirectX::CXMMATRIX _M)					{ m_texTransform->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetBoneTransforms(const DirectX::XMFLOAT4X4* _M, int _cnt){ m_boneTransforms->SetMatrixArray(reinterpret_cast<const float*>(_M), 0, _cnt); }

	void SetExplodeTime(float _f)                        { m_ExplodeTime->SetFloat(_f); }

	void SetMaterial(const DXMaterial& mat)               { m_mat->SetRawValue(&mat, 0, sizeof(DXMaterial)); }

	void SetDiffuseTexture(ID3D11ShaderResourceView* _tex)   { m_diffuseTex->SetResource(_tex); }
	void SetBlendTexture(ID3D11ShaderResourceView* _tex)    { m_blendTex->SetResource(_tex); }
	void SetNormalTexture(ID3D11ShaderResourceView* _tex)    { m_normalTex->SetResource(_tex); }

	ID3DX11EffectTechnique* m_basicTech;
	ID3DX11EffectTechnique* m_texTech;
	ID3DX11EffectTechnique* m_texNormalTech;
	ID3DX11EffectTechnique* m_texAlphaClipTech;
	ID3DX11EffectTechnique* m_texNormalAlphaClipTech;
	ID3DX11EffectTechnique* m_normalTech;

	ID3DX11EffectTechnique* m_basicSkinnedTech;
	ID3DX11EffectTechnique* m_texSkinnedTech;
	ID3DX11EffectTechnique* m_texNormalSkinnedTech;
	ID3DX11EffectTechnique* m_texAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* m_texNormalAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* m_normalSkinnedTech;

	ID3DX11EffectTechnique* m_basicInstancedTech;
	ID3DX11EffectTechnique* m_texInstancedTech;
	ID3DX11EffectTechnique* m_texNormalInstancedTech;
	ID3DX11EffectTechnique* m_normalInstancedTech;

	ID3DX11EffectMatrixVariable* m_worldViewProj;
	ID3DX11EffectMatrixVariable* m_world;
	ID3DX11EffectMatrixVariable* m_viewProj;
	ID3DX11EffectMatrixVariable* m_worldInvTranspose;
	ID3DX11EffectMatrixVariable* m_texTransform;
	ID3DX11EffectMatrixVariable* m_boneTransforms;

	ID3DX11EffectScalarVariable* m_ExplodeTime;

	ID3DX11EffectVariable* m_mat;

	ID3DX11EffectShaderResourceVariable* m_diffuseTex;
	ID3DX11EffectShaderResourceVariable* m_blendTex;
	ID3DX11EffectShaderResourceVariable* m_normalTex;
};
#pragma endregion


#pragma region TiledLightningEffect
class TiledLightningEffect : public DXEffect
{
public:
	TiledLightningEffect(ID3D11Device* _device, const std::wstring& _filename);
	~TiledLightningEffect();

	//void SetViewProjTexs(const DirectX::XMFLOAT4X4* _M, int _cnt)		{ m_viewProjTexs->SetMatrixArray(reinterpret_cast<const float*>(_M), 0, _cnt); }
	//void SetViewProj(const DirectX::XMFLOAT4X4* _M, int _cnt)			{ m_viewProj->SetMatrixArray(reinterpret_cast<const float*>(_M), 0, _cnt); }
	//void SetTexs(const DirectX::XMFLOAT4X4* _M, int _cnt)				{ m_texs->SetMatrixArray(reinterpret_cast<const float*>(_M), 0, _cnt); }
	void SetInvViewProj(const DirectX::XMFLOAT4X4* _M)					{ m_invViewProj->SetMatrix(reinterpret_cast<const float*>(_M)); }
	void SetCamPosition(const DirectX::XMFLOAT4* _V)					{ m_camPosition->SetFloatVector(reinterpret_cast<const float*>(_V)); }
	void SetResolution(const DirectX::XMFLOAT2 _v)					{ m_resolution->SetRawValue(&_v, 0, sizeof(DirectX::XMFLOAT2)); }
	//void SetShadowMapSwitches(const DirectX::XMFLOAT3 _v)				{ m_shadowMapSwitches->SetRawValue(&_v, 0, sizeof(DirectX::XMFLOAT3)); }
	//void SetShadowMapResolution(const DirectX::XMFLOAT2 _v)			{ m_shadowMapResolution->SetRawValue(&_v, 0, sizeof(DirectX::XMFLOAT2)); }
	//void SetGlobalLight(const DirectX::XMFLOAT3 _v)					{ m_globalLight->SetRawValue(&_v, 0, sizeof(DirectX::XMFLOAT3)); }

	void SetAlbedoMap(ID3D11ShaderResourceView* _tex)		{ m_albedoMap->SetResource(_tex); }
	void SetNormalSpecMap(ID3D11ShaderResourceView* _tex)    { m_normalSpecMap->SetResource(_tex); }
	void SetDepthMap(ID3D11ShaderResourceView* _tex)			{ m_depthMap->SetResource(_tex); }
	//void SetShadowMap0(ID3D11ShaderResourceView* _tex)		{ m_shadowMap0->SetResource(_tex); }
	//void SetShadowMap1(ID3D11ShaderResourceView* _tex)		{ m_shadowMap1->SetResource(_tex); }
	//void SetShadowMap2(ID3D11ShaderResourceView* _tex)		{ m_shadowMap2->SetResource(_tex); }
	//void SetShadowMap3(ID3D11ShaderResourceView* _tex)		{ m_shadowMap3->SetResource(_tex); }
	void SetOutputMap(ID3D11UnorderedAccessView* _tex)		{ m_outputMap->SetUnorderedAccessView(_tex); }
	void SetDirLightMap(ID3D11ShaderResourceView* _tex)		{ m_dirLightMap->SetResource(_tex); }
	void SetPointLightMap(ID3D11ShaderResourceView* _tex)	{ m_pointLightMap->SetResource(_tex); }
	void SetSpotLightMap(ID3D11ShaderResourceView* _tex)		{ m_spotLightMap->SetResource(_tex); }

	ID3DX11EffectTechnique* m_basicTech;

	//ID3DX11EffectMatrixVariable* m_viewProjTexs;
	//ID3DX11EffectMatrixVariable* m_viewProj;
	//ID3DX11EffectMatrixVariable* m_texs;
	ID3DX11EffectMatrixVariable* m_invViewProj;	
	ID3DX11EffectVectorVariable* m_camPosition;
	ID3DX11EffectVectorVariable* m_resolution;
	//ID3DX11EffectVectorVariable* m_shadowMapSwitches;
	//ID3DX11EffectVectorVariable* m_shadowMapResolution;
	//ID3DX11EffectVectorVariable* m_globalLight;

	ID3DX11EffectShaderResourceVariable* m_albedoMap;
	ID3DX11EffectShaderResourceVariable* m_normalSpecMap;
	ID3DX11EffectShaderResourceVariable* m_depthMap;
	//ID3DX11EffectShaderResourceVariable* m_shadowMap0;
	//ID3DX11EffectShaderResourceVariable* m_shadowMap1;
	//ID3DX11EffectShaderResourceVariable* m_shadowMap2;
	//ID3DX11EffectShaderResourceVariable* m_shadowMap3;
	ID3DX11EffectUnorderedAccessViewVariable* m_outputMap;

	ID3DX11EffectShaderResourceVariable* m_dirLightMap;
	ID3DX11EffectShaderResourceVariable* m_pointLightMap;
	ID3DX11EffectShaderResourceVariable* m_spotLightMap;
};
#pragma endregion


#pragma region BuildShadowMapEffect
class BuildShadowMapEffect : public DXEffect
{
public:
	BuildShadowMapEffect(ID3D11Device* _device, const std::wstring& _filename);
	~BuildShadowMapEffect();

	void SetViewProj(DirectX::CXMMATRIX _M)                       { m_viewProj->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetWorldViewProj(DirectX::CXMMATRIX _M)                  { m_worldViewProj->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetWorld(DirectX::CXMMATRIX _M)                          { m_world->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX _M)              { m_worldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetBoneTransforms(const DirectX::XMFLOAT4X4* _M, int _cnt){ m_boneTransforms->SetMatrixArray(reinterpret_cast<const float*>(_M), 0, _cnt); }
	void SetTexTransform(DirectX::CXMMATRIX _M)                   { m_texTransform->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& _v)                  { m_eyePosW->SetRawValue(&_v, 0, sizeof(DirectX::XMFLOAT3)); }
	
	void SetHeightScale(float _f)                        { m_heightScale->SetFloat(_f); }
	void SetMaxTessDistance(float _f)                    { m_maxTessDistance->SetFloat(_f); }
	void SetMinTessDistance(float _f)                    { m_minTessDistance->SetFloat(_f); }
	void SetMinTessFactor(float _f)                      { m_minTessFactor->SetFloat(_f); }
	void SetMaxTessFactor(float _f)                      { m_maxTessFactor->SetFloat(_f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* _tex)   { m_diffuseMap->SetResource(_tex); }
	void SetNormalMap(ID3D11ShaderResourceView* _tex)    { m_normalMap->SetResource(_tex); }

	ID3DX11EffectTechnique* m_buildShadowMapTech;
	ID3DX11EffectTechnique* m_buildShadowMapAlphaClipTech;
	ID3DX11EffectTechnique* m_buildShadowMapSkinnedTech;
	ID3DX11EffectTechnique* m_buildShadowMapAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* m_tessBuildShadowMapTech;
	ID3DX11EffectTechnique* m_tessBuildShadowMapAlphaClipTech;

	ID3DX11EffectMatrixVariable* m_viewProj;
	ID3DX11EffectMatrixVariable* m_worldViewProj;
	ID3DX11EffectMatrixVariable* m_world;
	ID3DX11EffectMatrixVariable* m_worldInvTranspose;
	ID3DX11EffectMatrixVariable* m_boneTransforms;
	ID3DX11EffectMatrixVariable* m_texTransform;
	ID3DX11EffectVectorVariable* m_eyePosW;
	ID3DX11EffectScalarVariable* m_heightScale;
	ID3DX11EffectScalarVariable* m_maxTessDistance;
	ID3DX11EffectScalarVariable* m_minTessDistance;
	ID3DX11EffectScalarVariable* m_minTessFactor;
	ID3DX11EffectScalarVariable* m_maxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* m_diffuseMap;
	ID3DX11EffectShaderResourceVariable* m_normalMap;
};
#pragma endregion




#pragma region ClearGBufferEffect
class ClearGBufferEffect : public DXEffect
{
public:
	ClearGBufferEffect(ID3D11Device* _device, const std::wstring& _filename);
	~ClearGBufferEffect();

	ID3DX11EffectTechnique* ClearTech;
};
#pragma endregion

#pragma region ShadowMapEffect
class ShadowMapEffect : public DXEffect
{
public:
	ShadowMapEffect(ID3D11Device* _device, const std::wstring& _filename);
	~ShadowMapEffect();

	void SetTexTransform(DirectX::CXMMATRIX _M)					{ m_texTransform->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetWorldViewProj(DirectX::CXMMATRIX _M)					{ m_worldViewProj->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetBoneTransforms(const DirectX::XMFLOAT4X4* _M, int _cnt){ m_boneTransforms->SetMatrixArray(reinterpret_cast<const float*>(_M), 0, _cnt); }
	void SetDiffuseMap(ID3D11ShaderResourceView* _tex)   { m_diffuseMap->SetResource(_tex); }

	ID3DX11EffectTechnique* m_basicShadowDirTech;
	ID3DX11EffectTechnique* m_basicShadowPointTech;
	ID3DX11EffectTechnique* m_basicShadowSpotTech;

	ID3DX11EffectTechnique* m_alphaClipShadowDirTech;
	ID3DX11EffectTechnique* m_alphaClipShadowPointTech;
	ID3DX11EffectTechnique* m_alphaClipShadowSpotTech;

	//animation
	ID3DX11EffectTechnique* m_skinnedBasicShadowDirTech;
	ID3DX11EffectTechnique* m_skinnedBasicShadowPointTech;
	ID3DX11EffectTechnique* m_skinnedBasicShadowSpotTech;

	ID3DX11EffectTechnique* m_skinnedAlphaClipShadowDirTech;
	ID3DX11EffectTechnique* m_skinnedAlphaClipShadowPointTech;
	ID3DX11EffectTechnique* m_skinnedAlphaClipShadowSpotTech;



	ID3DX11EffectMatrixVariable* m_texTransform;
	ID3DX11EffectMatrixVariable* m_worldViewProj;
	ID3DX11EffectMatrixVariable* m_boneTransforms;

	ID3DX11EffectShaderResourceVariable* m_diffuseMap;
};
#pragma endregion

#pragma region CombineFinalEffect
class CombineFinalEffect : public DXEffect
{
public:
	CombineFinalEffect(ID3D11Device* _device, const std::wstring& _filename);
	~CombineFinalEffect();

	void SetOpacity(float _value)					{ m_opacity->SetFloat(_value); }
	void SetColor(const DirectX::XMFLOAT3& _v)                  { m_color->SetRawValue(&_v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetTexture(ID3D11ShaderResourceView* _tex)  { m_texture->SetResource(_tex); }
	void SetIndex(float _value)					{ m_index->SetFloat(_value); }
	void SetNumSymbols(float _value)					{ m_numSymbols->SetFloat(_value); }

	ID3DX11EffectTechnique* m_monoTech;
	ID3DX11EffectTechnique* m_colorTech;
	ID3DX11EffectTechnique* m_alphaTransparencyColorTech;
	ID3DX11EffectTechnique* m_transparencyColorTech;
	ID3DX11EffectTechnique* m_alphaClipColorTech;
	ID3DX11EffectTechnique* m_textTech;
		
	ID3DX11EffectScalarVariable*		m_opacity;
	ID3DX11EffectVectorVariable*		m_color;

	ID3DX11EffectScalarVariable*		m_index;
	ID3DX11EffectScalarVariable*		m_numSymbols;

	ID3DX11EffectShaderResourceVariable* m_texture;
};
#pragma endregion

#pragma region RenderTextEffect
class RenderTextEffect : public DXEffect
{
public:
	RenderTextEffect(ID3D11Device* _device, const std::wstring& _filename);
	~RenderTextEffect();

	void SetPosition(const DirectX::XMFLOAT2* _V)					{ m_position->SetFloatVector(reinterpret_cast<const float*>(_V)); }
	void SetColor(const DirectX::XMFLOAT4* _V)	{ m_color->SetFloatVector(reinterpret_cast<const float*>(_V)); }
	void SetTexture(ID3D11ShaderResourceView* _tex)  { m_texture->SetResource(_tex); }
	void SetOutputMap(ID3D11UnorderedAccessView* _tex)		{ m_outputMap->SetUnorderedAccessView(_tex); }
	

	ID3DX11EffectTechnique* m_basicTech;

	ID3DX11EffectVectorVariable*				m_position;
	ID3DX11EffectVectorVariable*				m_color;
	ID3DX11EffectShaderResourceVariable*		m_texture;
	ID3DX11EffectUnorderedAccessViewVariable*	m_outputMap;
};
#pragma endregion


#pragma region SkyEffect
class SkyEffect : public DXEffect
{
public:
	SkyEffect(ID3D11Device* _device, const std::wstring& _filename);
	~SkyEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX _M)					{ m_WVP->SetMatrix(reinterpret_cast<const float*>(&_M)); }
	void SetCubeMap(ID3D11ShaderResourceView* _tex)		{ m_cubemap->SetResource(_tex); }


	ID3DX11EffectTechnique* m_skyTech;

	ID3DX11EffectMatrixVariable*				m_WVP;
	ID3DX11EffectShaderResourceVariable*		m_cubemap;
};
#pragma endregion

#pragma region Effects
class DXEffects
{
public:
	static void InitAll(ID3D11Device* _device);
	static void DestroyAll();

	static CombineFinalEffect* m_combineFinalFX;
	static ObjectDeferredEffect* m_objectDeferredFX;
	static TiledLightningEffect* m_tiledLightningFX;
	static RenderTextEffect*	m_renderTextFX;
	static SkyEffect*			m_skyFX;
};
#pragma endregion



#endif

