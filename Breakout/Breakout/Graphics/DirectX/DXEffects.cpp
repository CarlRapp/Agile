#include "DXEffects.h"


#pragma region DXEffect
DXEffect::DXEffect(ID3D11Device* _device, const std::wstring& _filename)
	: m_FX(0)
{
	std::ifstream fin(_filename.c_str(), std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HRESULT(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, _device, &m_FX));
}

DXEffect::~DXEffect()
{
	ReleaseCOM(m_FX);
}
#pragma endregion


#pragma region ObjectDeferredEffect
ObjectDeferredEffect::ObjectDeferredEffect(ID3D11Device* _device, const std::wstring& _filename)
	: DXEffect(_device, _filename)
{
	m_basicTech = m_FX->GetTechniqueByName("BasicTech");
	m_texTech = m_FX->GetTechniqueByName("TexTech");
	m_texNormalTech = m_FX->GetTechniqueByName("TexNormalTech");
	m_texAlphaClipTech = m_FX->GetTechniqueByName("TexAlphaClipTech");
	m_texNormalAlphaClipTech = m_FX->GetTechniqueByName("TexNormalAlphaClipTech");
	m_normalTech = m_FX->GetTechniqueByName("NormalTech");

	m_basicSkinnedTech = m_FX->GetTechniqueByName("BasicSkinnedTech");
	m_texSkinnedTech = m_FX->GetTechniqueByName("TexSkinnedTech");
	m_texNormalSkinnedTech = m_FX->GetTechniqueByName("TexNormalSkinnedTech");
	m_texAlphaClipSkinnedTech = m_FX->GetTechniqueByName("TexAlphaClipSkinnedTech");
	m_texNormalAlphaClipSkinnedTech = m_FX->GetTechniqueByName("TexNormalAlphaClipSkinnedTech");
	m_normalSkinnedTech = m_FX->GetTechniqueByName("NormalSkinnedTech");

	m_basicInstancedTech = m_FX->GetTechniqueByName("BasicInstancedTech");
	m_texInstancedTech = m_FX->GetTechniqueByName("TexInstancedTech");
	m_texNormalInstancedTech = m_FX->GetTechniqueByName("TexNormalInstancedTech");
	m_normalInstancedTech = m_FX->GetTechniqueByName("NormalInstancedTech");

	m_worldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_world = m_FX->GetVariableByName("gWorld")->AsMatrix();
	m_viewProj = m_FX->GetVariableByName("gViewProj")->AsMatrix();
	m_worldInvTranspose = m_FX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_texTransform = m_FX->GetVariableByName("gTexTransform")->AsMatrix();
	m_boneTransforms = m_FX->GetVariableByName("gBoneTransforms")->AsMatrix();

	m_mat = m_FX->GetVariableByName("gMaterial");

	m_diffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_normalMap = m_FX->GetVariableByName("gNormalMap")->AsShaderResource();
}

ObjectDeferredEffect::~ObjectDeferredEffect()
{
}
#pragma endregion



#pragma region TiledLightningEffect
TiledLightningEffect::TiledLightningEffect(ID3D11Device* _device, const std::wstring& _filename)
	: DXEffect(_device, _filename)
{
	m_basicTech = m_FX->GetTechniqueByName("Basic");

	//m_viewProjTexs = m_FX->GetVariableByName("gLightViewProjTex")->AsMatrix();
	//m_viewProj = m_FX->GetVariableByName("gLightViewProj")->AsMatrix();
	//m_texs = m_FX->GetVariableByName("gLightTex")->AsMatrix();
	m_invViewProj = m_FX->GetVariableByName("gInvViewProj")->AsMatrix();
	m_camPosition = m_FX->GetVariableByName("gCamPosition")->AsVector();
	m_resolution = m_FX->GetVariableByName("gResolution")->AsVector();
	//m_shadowMapSwitches = m_FX->GetVariableByName("gShadowMapSwitches")->AsVector();
	//m_shadowMapResolution = m_FX->GetVariableByName("gShadowMapResolution")->AsVector();
	//m_globalLight = m_FX->GetVariableByName("gGlobalLight")->AsVector();

	m_albedoMap = m_FX->GetVariableByName("gAlbedoMap")->AsShaderResource();
	m_normalSpecMap = m_FX->GetVariableByName("gNormalSpecMap")->AsShaderResource();
	m_depthMap = m_FX->GetVariableByName("gDepthMap")->AsShaderResource();
	//m_shadowMap0 = m_FX->GetVariableByName("gShadowMap0")->AsShaderResource();
	//m_shadowMap1 = m_FX->GetVariableByName("gShadowMap1")->AsShaderResource();
	//m_shadowMap2 = m_FX->GetVariableByName("gShadowMap2")->AsShaderResource();
	//m_shadowMap3 = m_FX->GetVariableByName("gShadowMap3")->AsShaderResource();
	m_outputMap = m_FX->GetVariableByName("gOutput")->AsUnorderedAccessView();

	m_dirLightMap = m_FX->GetVariableByName("gDirLightBuffer")->AsShaderResource();
	m_pointLightMap = m_FX->GetVariableByName("gPointLightBuffer")->AsShaderResource();
	m_spotLightMap = m_FX->GetVariableByName("gSpotLightBuffer")->AsShaderResource();
}

TiledLightningEffect::~TiledLightningEffect()
{
}
#pragma endregion



#pragma region BuildShadowMapEffect
BuildShadowMapEffect::BuildShadowMapEffect(ID3D11Device* _device, const std::wstring& _filename)
	: DXEffect(_device, _filename)
{
	m_buildShadowMapTech = m_FX->GetTechniqueByName("BuildShadowMapTech");
	m_buildShadowMapAlphaClipTech = m_FX->GetTechniqueByName("BuildShadowMapAlphaClipTech");

	m_buildShadowMapSkinnedTech = m_FX->GetTechniqueByName("BuildShadowMapSkinnedTech");
	m_buildShadowMapAlphaClipSkinnedTech = m_FX->GetTechniqueByName("BuildShadowMapAlphaClipSkinnedTech");

	m_tessBuildShadowMapTech = m_FX->GetTechniqueByName("TessBuildShadowMapTech");
	m_tessBuildShadowMapAlphaClipTech = m_FX->GetTechniqueByName("TessBuildShadowMapAlphaClipTech");
	
	m_viewProj = m_FX->GetVariableByName("gViewProj")->AsMatrix();
	m_worldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_world = m_FX->GetVariableByName("gWorld")->AsMatrix();
	m_worldInvTranspose = m_FX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_boneTransforms = m_FX->GetVariableByName("gBoneTransforms")->AsMatrix();
	m_texTransform = m_FX->GetVariableByName("gTexTransform")->AsMatrix();
	m_eyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
	m_heightScale = m_FX->GetVariableByName("gHeightScale")->AsScalar();
	m_maxTessDistance = m_FX->GetVariableByName("gMaxTessDistance")->AsScalar();
	m_minTessDistance = m_FX->GetVariableByName("gMinTessDistance")->AsScalar();
	m_minTessFactor = m_FX->GetVariableByName("gMinTessFactor")->AsScalar();
	m_maxTessFactor = m_FX->GetVariableByName("gMaxTessFactor")->AsScalar();
	m_diffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_normalMap = m_FX->GetVariableByName("gNormalMap")->AsShaderResource();
}

BuildShadowMapEffect::~BuildShadowMapEffect()
{
}
#pragma endregion










#pragma region ClearGBufferEffect
ClearGBufferEffect::ClearGBufferEffect(ID3D11Device* _device, const std::wstring& _filename)
	: DXEffect(_device, _filename)
{
	ClearTech = m_FX->GetTechniqueByName("ClearTech");
}

ClearGBufferEffect::~ClearGBufferEffect()
{
}
#pragma endregion

#pragma region ShadowMapEffect
ShadowMapEffect::ShadowMapEffect(ID3D11Device* _device, const std::wstring& _filename)
	: DXEffect(_device, _filename)
{
	m_basicShadowDirTech = m_FX->GetTechniqueByName("BasicShadowDir");
	m_basicShadowPointTech = m_FX->GetTechniqueByName("BasicShadowPoint");
	m_basicShadowSpotTech = m_FX->GetTechniqueByName("BasicShadowSpot");
	m_alphaClipShadowDirTech = m_FX->GetTechniqueByName("AlphaClipShadowDir");
	m_alphaClipShadowPointTech = m_FX->GetTechniqueByName("AlphaClipShadowPoint");
	m_alphaClipShadowSpotTech = m_FX->GetTechniqueByName("AlphaClipShadowSpot");

	m_skinnedBasicShadowDirTech = m_FX->GetTechniqueByName("SkinnedBasicShadowDir");
	m_skinnedBasicShadowPointTech = m_FX->GetTechniqueByName("SkinnedBasicShadowPoint");
	m_skinnedBasicShadowSpotTech = m_FX->GetTechniqueByName("SkinnedBasicShadowSpot");
	m_skinnedAlphaClipShadowDirTech = m_FX->GetTechniqueByName("SkinnedAlphaClipShadowDir");
	m_skinnedAlphaClipShadowPointTech = m_FX->GetTechniqueByName("SkinnedAlphaClipShadowPoint");
	m_skinnedAlphaClipShadowSpotTech = m_FX->GetTechniqueByName("SkinnedAlphaClipShadowSpot");

	m_diffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	m_texTransform = m_FX->GetVariableByName("gTexTransform")->AsMatrix();
	m_worldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_boneTransforms = m_FX->GetVariableByName("gBoneTransforms")->AsMatrix();
}

ShadowMapEffect::~ShadowMapEffect()
{
}
#pragma endregion

#pragma region CombineFinalEffect
CombineFinalEffect::CombineFinalEffect(ID3D11Device* _device, const std::wstring& _filename)
	: DXEffect(_device, _filename)
{
	m_monoTech = m_FX->GetTechniqueByName("Mono");
	m_colorTech = m_FX->GetTechniqueByName("Color");
	m_transparencyColorTech = m_FX->GetTechniqueByName("TransparencyColor");
	m_alphaTransparencyColorTech = m_FX->GetTechniqueByName("AlphaTransparencyColor");
	m_alphaClipColorTech = m_FX->GetTechniqueByName("AlphaClipColor");

	m_opacity = m_FX->GetVariableByName("g_Opacity")->AsScalar();

	m_texture = m_FX->GetVariableByName("g_Texture")->AsShaderResource();
}

CombineFinalEffect::~CombineFinalEffect()
{
}
#pragma endregion

#pragma region DXEffects

BuildShadowMapEffect*  DXEffects::m_buildShadowMapFX = 0;
ClearGBufferEffect*    DXEffects::m_clearGBufferFX = 0;
CombineFinalEffect*	   DXEffects::m_combineFinalFX = 0;
ObjectDeferredEffect*  DXEffects::m_objectDeferredFX = 0;
TiledLightningEffect*  DXEffects::m_tiledLightningFX = 0;
ShadowMapEffect*	   DXEffects::m_shadowMapFX = 0;

void DXEffects::InitAll(ID3D11Device* _device)
{
	m_buildShadowMapFX = new BuildShadowMapEffect(_device, L"Graphics/DirectX/Shaders/BuildShadowMap.fxo");
	m_clearGBufferFX = new ClearGBufferEffect(_device, L"Graphics/DirectX/Shaders/ClearGBuffer.fxo");
	m_combineFinalFX = new CombineFinalEffect(_device, L"Graphics/DirectX/Shaders/CombineFinal.fxo");
	m_objectDeferredFX = new ObjectDeferredEffect(_device, L"Graphics/DirectX/Shaders/ObjectDeferred.fxo");
	m_tiledLightningFX = new TiledLightningEffect(_device, L"Graphics/DirectX/Shaders/TiledLightning.fxo");
	m_shadowMapFX = new ShadowMapEffect(_device, L"Graphics/DirectX/Shaders/ShadowMap.fxo");
}

void DXEffects::DestroyAll()
{
	SafeDelete(m_buildShadowMapFX);
	SafeDelete(m_clearGBufferFX);
	SafeDelete(m_combineFinalFX);
	SafeDelete(m_objectDeferredFX);
	SafeDelete(m_tiledLightningFX);
	SafeDelete(m_shadowMapFX);
}

#pragma endregion