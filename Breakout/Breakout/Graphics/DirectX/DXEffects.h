#pragma once
#ifndef DXEFFECTS_H
#define DXEFFECTS_H


#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { delete x; x = 0; }

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

#pragma comment(lib, "Effects11D.lib")

#pragma region DXEffect
class DXEffect
{
public:
	DXEffect(ID3D11Device* device, const std::wstring& filename);
	virtual ~DXEffect();

private:
	DXEffect(const DXEffect& rhs);
	DXEffect& operator=(const DXEffect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion





#pragma region ObjectDeferredEffect
class ObjectDeferredEffect : public DXEffect
{
public:
	ObjectDeferredEffect(ID3D11Device* device, const std::wstring& filename);
	~ObjectDeferredEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX M)					{ WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M)							{ World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M)				{ WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(DirectX::CXMMATRIX M)					{ TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetBoneTransforms(const DirectX::XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }

	//void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }

	ID3DX11EffectTechnique* BasicTech;
	ID3DX11EffectTechnique* TexTech;
	ID3DX11EffectTechnique* TexNormalTech;
	ID3DX11EffectTechnique* TexAlphaClipTech;
	ID3DX11EffectTechnique* TexNormalAlphaClipTech;
	ID3DX11EffectTechnique* NormalTech;

	ID3DX11EffectTechnique* BasicSkinnedTech;
	ID3DX11EffectTechnique* TexSkinnedTech;
	ID3DX11EffectTechnique* TexNormalSkinnedTech;
	ID3DX11EffectTechnique* TexAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* TexNormalAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* NormalSkinnedTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectMatrixVariable* BoneTransforms;

	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};
#pragma endregion


#pragma region TiledLightningEffect
class TiledLightningEffect : public DXEffect
{
public:
	TiledLightningEffect(ID3D11Device* device, const std::wstring& filename);
	~TiledLightningEffect();

	void SetViewProjTexs(const DirectX::XMFLOAT4X4* M, int cnt)		{ ViewProjTexs->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetViewProj(const DirectX::XMFLOAT4X4* M, int cnt)			{ ViewProj->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetTexs(const DirectX::XMFLOAT4X4* M, int cnt)				{ Texs->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetInvViewProjs(const DirectX::XMFLOAT4X4* M, int cnt)		{ InvViewProjs->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetCamPositions(const DirectX::XMFLOAT4* V, int cnt)		{ CamPositions->SetFloatVectorArray(reinterpret_cast<const float*>(V), 0, cnt); }
	void SetResolution(const DirectX::XMFLOAT2 v)					{ Resolution->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT2)); }
	void SetShadowMapSwitches(const DirectX::XMFLOAT3 v)				{ ShadowMapSwitches->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetShadowMapResolution(const DirectX::XMFLOAT2 v)			{ ShadowMapResolution->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT2)); }
	void SetGlobalLight(const DirectX::XMFLOAT3 v)					{ GlobalLight->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }

	void SetAlbedoMap(ID3D11ShaderResourceView* tex)		{ AlbedoMap->SetResource(tex); }
	void SetNormalSpecMap(ID3D11ShaderResourceView* tex)    { NormalSpecMap->SetResource(tex); }
	void SetDepthMap(ID3D11ShaderResourceView* tex)			{ DepthMap->SetResource(tex); }
	void SetShadowMap0(ID3D11ShaderResourceView* tex)		{ ShadowMap0->SetResource(tex); }
	void SetShadowMap1(ID3D11ShaderResourceView* tex)		{ ShadowMap1->SetResource(tex); }
	void SetShadowMap2(ID3D11ShaderResourceView* tex)		{ ShadowMap2->SetResource(tex); }
	void SetShadowMap3(ID3D11ShaderResourceView* tex)		{ ShadowMap3->SetResource(tex); }
	void SetOutputMap(ID3D11UnorderedAccessView* tex)		{ OutputMap->SetUnorderedAccessView(tex); }
	void SetDirLightMap(ID3D11ShaderResourceView* tex)		{ DirLightMap->SetResource(tex); }
	void SetPointLightMap(ID3D11ShaderResourceView* tex)	{ PointLightMap->SetResource(tex); }
	void SetSpotLightMap(ID3D11ShaderResourceView* tex)		{ SpotLightMap->SetResource(tex); }

	ID3DX11EffectTechnique* Viewport1;
	ID3DX11EffectTechnique* Viewport2;
	ID3DX11EffectTechnique* Viewport3;
	ID3DX11EffectTechnique* Viewport4;

	ID3DX11EffectMatrixVariable* ViewProjTexs;
	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* Texs;
	ID3DX11EffectMatrixVariable* InvViewProjs;	
	ID3DX11EffectVectorVariable* CamPositions;
	ID3DX11EffectVectorVariable* Resolution;
	ID3DX11EffectVectorVariable* ShadowMapSwitches;
	ID3DX11EffectVectorVariable* ShadowMapResolution;
	ID3DX11EffectVectorVariable* GlobalLight;

	ID3DX11EffectShaderResourceVariable* AlbedoMap;
	ID3DX11EffectShaderResourceVariable* NormalSpecMap;
	ID3DX11EffectShaderResourceVariable* DepthMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap0;
	ID3DX11EffectShaderResourceVariable* ShadowMap1;
	ID3DX11EffectShaderResourceVariable* ShadowMap2;
	ID3DX11EffectShaderResourceVariable* ShadowMap3;
	ID3DX11EffectUnorderedAccessViewVariable* OutputMap;

	ID3DX11EffectShaderResourceVariable* DirLightMap;
	ID3DX11EffectShaderResourceVariable* PointLightMap;
	ID3DX11EffectShaderResourceVariable* SpotLightMap;
};
#pragma endregion


#pragma region BuildShadowMapEffect
class BuildShadowMapEffect : public DXEffect
{
public:
	BuildShadowMapEffect(ID3D11Device* device, const std::wstring& filename);
	~BuildShadowMapEffect();

	void SetViewProj(DirectX::CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(DirectX::CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetBoneTransforms(const DirectX::XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetTexTransform(DirectX::CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	
	void SetHeightScale(float f)                        { HeightScale->SetFloat(f); }
	void SetMaxTessDistance(float f)                    { MaxTessDistance->SetFloat(f); }
	void SetMinTessDistance(float f)                    { MinTessDistance->SetFloat(f); }
	void SetMinTessFactor(float f)                      { MinTessFactor->SetFloat(f); }
	void SetMaxTessFactor(float f)                      { MaxTessFactor->SetFloat(f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }

	ID3DX11EffectTechnique* BuildShadowMapTech;
	ID3DX11EffectTechnique* BuildShadowMapAlphaClipTech;
	ID3DX11EffectTechnique* BuildShadowMapSkinnedTech;
	ID3DX11EffectTechnique* BuildShadowMapAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* TessBuildShadowMapTech;
	ID3DX11EffectTechnique* TessBuildShadowMapAlphaClipTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectScalarVariable* HeightScale;
	ID3DX11EffectScalarVariable* MaxTessDistance;
	ID3DX11EffectScalarVariable* MinTessDistance;
	ID3DX11EffectScalarVariable* MinTessFactor;
	ID3DX11EffectScalarVariable* MaxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};
#pragma endregion




#pragma region ClearGBufferEffect
class ClearGBufferEffect : public DXEffect
{
public:
	ClearGBufferEffect(ID3D11Device* device, const std::wstring& filename);
	~ClearGBufferEffect();

	ID3DX11EffectTechnique* ClearTech;
};
#pragma endregion

#pragma region ShadowMapEffect
class ShadowMapEffect : public DXEffect
{
public:
	ShadowMapEffect(ID3D11Device* device, const std::wstring& filename);
	~ShadowMapEffect();

	void SetTexTransform(DirectX::CXMMATRIX M)					{ TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(DirectX::CXMMATRIX M)					{ WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetBoneTransforms(const DirectX::XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }

	ID3DX11EffectTechnique* BasicShadowDirTech;
	ID3DX11EffectTechnique* BasicShadowPointTech;
	ID3DX11EffectTechnique* BasicShadowSpotTech;

	ID3DX11EffectTechnique* AlphaClipShadowDirTech;
	ID3DX11EffectTechnique* AlphaClipShadowPointTech;
	ID3DX11EffectTechnique* AlphaClipShadowSpotTech;

	//animation
	ID3DX11EffectTechnique* SkinnedBasicShadowDirTech;
	ID3DX11EffectTechnique* SkinnedBasicShadowPointTech;
	ID3DX11EffectTechnique* SkinnedBasicShadowSpotTech;

	ID3DX11EffectTechnique* SkinnedAlphaClipShadowDirTech;
	ID3DX11EffectTechnique* SkinnedAlphaClipShadowPointTech;
	ID3DX11EffectTechnique* SkinnedAlphaClipShadowSpotTech;



	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* BoneTransforms;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};
#pragma endregion

#pragma region CombineFinalEffect
class CombineFinalEffect : public DXEffect
{
public:
	CombineFinalEffect(ID3D11Device* device, const std::wstring& filename);
	~CombineFinalEffect();

	void SetOpacity(float value)					{ Opacity->SetFloat(value); }
	void SetTexture(ID3D11ShaderResourceView* tex)  { Texture->SetResource(tex); }

	ID3DX11EffectTechnique* MonoTech;
	ID3DX11EffectTechnique* ColorTech;
	ID3DX11EffectTechnique* AlphaTransparencyColorTech;
	ID3DX11EffectTechnique* TransparencyColorTech;
	ID3DX11EffectTechnique* AlphaClipColorTech;
		
	ID3DX11EffectScalarVariable*		Opacity;

	ID3DX11EffectShaderResourceVariable* Texture;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();


	static BuildShadowMapEffect* BuildShadowMapFX;

	static ClearGBufferEffect* ClearGBufferFX;
	static CombineFinalEffect* CombineFinalFX;
	static ObjectDeferredEffect* ObjectDeferredFX;
	static TiledLightningEffect* TiledLightningFX;
	static ShadowMapEffect*	ShadowMapFX;
};
#pragma endregion



#endif

