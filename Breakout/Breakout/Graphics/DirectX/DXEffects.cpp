#include "DXEffects.h"


#pragma region DXEffect
DXEffect::DXEffect(ID3D11Device* device, const std::wstring& filename)
	: mFX(0)
{
	std::ifstream fin(filename.c_str(), std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HRESULT(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, device, &mFX));
}

DXEffect::~DXEffect()
{
	ReleaseCOM(mFX);
}
#pragma endregion


#pragma region BasicEffect
BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
	: DXEffect(device, filename)
{
	Light1Tech = mFX->GetTechniqueByName("Light1");
	Light2Tech = mFX->GetTechniqueByName("Light2");
	Light3Tech = mFX->GetTechniqueByName("Light3");

	Light0TexTech = mFX->GetTechniqueByName("Light0Tex");
	Light1TexTech = mFX->GetTechniqueByName("Light1Tex");
	Light2TexTech = mFX->GetTechniqueByName("Light2Tex");
	Light3TexTech = mFX->GetTechniqueByName("Light3Tex");

	Light0TexAlphaClipTech = mFX->GetTechniqueByName("Light0TexAlphaClip");
	Light1TexAlphaClipTech = mFX->GetTechniqueByName("Light1TexAlphaClip");
	Light2TexAlphaClipTech = mFX->GetTechniqueByName("Light2TexAlphaClip");
	Light3TexAlphaClipTech = mFX->GetTechniqueByName("Light3TexAlphaClip");

	Light1FogTech = mFX->GetTechniqueByName("Light1Fog");
	Light2FogTech = mFX->GetTechniqueByName("Light2Fog");
	Light3FogTech = mFX->GetTechniqueByName("Light3Fog");

	Light0TexFogTech = mFX->GetTechniqueByName("Light0TexFog");
	Light1TexFogTech = mFX->GetTechniqueByName("Light1TexFog");
	Light2TexFogTech = mFX->GetTechniqueByName("Light2TexFog");
	Light3TexFogTech = mFX->GetTechniqueByName("Light3TexFog");

	Light0TexAlphaClipFogTech = mFX->GetTechniqueByName("Light0TexAlphaClipFog");
	Light1TexAlphaClipFogTech = mFX->GetTechniqueByName("Light1TexAlphaClipFog");
	Light2TexAlphaClipFogTech = mFX->GetTechniqueByName("Light2TexAlphaClipFog");
	Light3TexAlphaClipFogTech = mFX->GetTechniqueByName("Light3TexAlphaClipFog");

	Light1ReflectTech = mFX->GetTechniqueByName("Light1Reflect");
	Light2ReflectTech = mFX->GetTechniqueByName("Light2Reflect");
	Light3ReflectTech = mFX->GetTechniqueByName("Light3Reflect");

	Light0TexReflectTech = mFX->GetTechniqueByName("Light0TexReflect");
	Light1TexReflectTech = mFX->GetTechniqueByName("Light1TexReflect");
	Light2TexReflectTech = mFX->GetTechniqueByName("Light2TexReflect");
	Light3TexReflectTech = mFX->GetTechniqueByName("Light3TexReflect");

	Light0TexAlphaClipReflectTech = mFX->GetTechniqueByName("Light0TexAlphaClipReflect");
	Light1TexAlphaClipReflectTech = mFX->GetTechniqueByName("Light1TexAlphaClipReflect");
	Light2TexAlphaClipReflectTech = mFX->GetTechniqueByName("Light2TexAlphaClipReflect");
	Light3TexAlphaClipReflectTech = mFX->GetTechniqueByName("Light3TexAlphaClipReflect");

	Light1FogReflectTech = mFX->GetTechniqueByName("Light1FogReflect");
	Light2FogReflectTech = mFX->GetTechniqueByName("Light2FogReflect");
	Light3FogReflectTech = mFX->GetTechniqueByName("Light3FogReflect");

	Light0TexFogReflectTech = mFX->GetTechniqueByName("Light0TexFogReflect");
	Light1TexFogReflectTech = mFX->GetTechniqueByName("Light1TexFogReflect");
	Light2TexFogReflectTech = mFX->GetTechniqueByName("Light2TexFogReflect");
	Light3TexFogReflectTech = mFX->GetTechniqueByName("Light3TexFogReflect");

	Light0TexAlphaClipFogReflectTech = mFX->GetTechniqueByName("Light0TexAlphaClipFogReflect");
	Light1TexAlphaClipFogReflectTech = mFX->GetTechniqueByName("Light1TexAlphaClipFogReflect");
	Light2TexAlphaClipFogReflectTech = mFX->GetTechniqueByName("Light2TexAlphaClipFogReflect");
	Light3TexAlphaClipFogReflectTech = mFX->GetTechniqueByName("Light3TexAlphaClipFogReflect");

	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	WorldViewProjTex = mFX->GetVariableByName("gWorldViewProjTex")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	ShadowTransform = mFX->GetVariableByName("gShadowTransform")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights = mFX->GetVariableByName("gDirLights");
	Mat = mFX->GetVariableByName("gMaterial");
	DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	CubeMap = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	ShadowMap = mFX->GetVariableByName("gShadowMap")->AsShaderResource();
	SsaoMap = mFX->GetVariableByName("gSsaoMap")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion


#pragma region ObjectDeferredEffect
ObjectDeferredEffect::ObjectDeferredEffect(ID3D11Device* device, const std::wstring& filename)
: DXEffect(device, filename)
{
	BasicTech				= mFX->GetTechniqueByName("BasicTech");
	TexTech					= mFX->GetTechniqueByName("TexTech");
	TexNormalTech			= mFX->GetTechniqueByName("TexNormalTech");
	TexAlphaClipTech		= mFX->GetTechniqueByName("TexAlphaClipTech");
	TexNormalAlphaClipTech	= mFX->GetTechniqueByName("TexNormalAlphaClipTech");
	NormalTech				= mFX->GetTechniqueByName("NormalTech");

	BasicSkinnedTech				= mFX->GetTechniqueByName("BasicSkinnedTech");
	TexSkinnedTech					= mFX->GetTechniqueByName("TexSkinnedTech");
	TexNormalSkinnedTech			= mFX->GetTechniqueByName("TexNormalSkinnedTech");
	TexAlphaClipSkinnedTech			= mFX->GetTechniqueByName("TexAlphaClipSkinnedTech");
	TexNormalAlphaClipSkinnedTech	= mFX->GetTechniqueByName("TexNormalAlphaClipSkinnedTech");
	NormalSkinnedTech				= mFX->GetTechniqueByName("NormalSkinnedTech");

	WorldViewProj			= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World					= mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose		= mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform			= mFX->GetVariableByName("gTexTransform")->AsMatrix();
	BoneTransforms			= mFX->GetVariableByName("gBoneTransforms")->AsMatrix();

	Mat						= mFX->GetVariableByName("gMaterial");

	DiffuseMap				= mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	NormalMap				= mFX->GetVariableByName("gNormalMap")->AsShaderResource();
}

ObjectDeferredEffect::~ObjectDeferredEffect()
{
}
#pragma endregion



#pragma region TiledLightningEffect
TiledLightningEffect::TiledLightningEffect(ID3D11Device* device, const std::wstring& filename)
: DXEffect(device, filename)
{
	Viewport1		= mFX->GetTechniqueByName("Viewport1");
	Viewport2		= mFX->GetTechniqueByName("Viewport2");
	Viewport3		= mFX->GetTechniqueByName("Viewport3");
	Viewport4		= mFX->GetTechniqueByName("Viewport4");

	ViewProjTexs		= mFX->GetVariableByName("gLightViewProjTex")->AsMatrix();
	ViewProj			= mFX->GetVariableByName("gLightViewProj")->AsMatrix();
	Texs				= mFX->GetVariableByName("gLightTex")->AsMatrix();
	InvViewProjs		= mFX->GetVariableByName("gInvViewProjs")->AsMatrix();
	CamPositions		= mFX->GetVariableByName("gCamPositions")->AsVector();
	Resolution			= mFX->GetVariableByName("gResolution")->AsVector();
	ShadowMapSwitches	= mFX->GetVariableByName("gShadowMapSwitches")->AsVector();
	ShadowMapResolution	= mFX->GetVariableByName("gShadowMapResolution")->AsVector();
	GlobalLight			= mFX->GetVariableByName("gGlobalLight")->AsVector();

	AlbedoMap		= mFX->GetVariableByName("gAlbedoMap")->AsShaderResource();
	NormalSpecMap   = mFX->GetVariableByName("gNormalSpecMap")->AsShaderResource();
	DepthMap		= mFX->GetVariableByName("gDepthMap")->AsShaderResource();
	ShadowMap0		= mFX->GetVariableByName("gShadowMap0")->AsShaderResource();
	ShadowMap1		= mFX->GetVariableByName("gShadowMap1")->AsShaderResource();
	ShadowMap2		= mFX->GetVariableByName("gShadowMap2")->AsShaderResource();
	ShadowMap3		= mFX->GetVariableByName("gShadowMap3")->AsShaderResource();
	OutputMap		= mFX->GetVariableByName("gOutput")->AsUnorderedAccessView();

	DirLightMap     = mFX->GetVariableByName("gDirLightBuffer")->AsShaderResource();
	PointLightMap   = mFX->GetVariableByName("gPointLightBuffer")->AsShaderResource();
	SpotLightMap    = mFX->GetVariableByName("gSpotLightBuffer")->AsShaderResource();
}

TiledLightningEffect::~TiledLightningEffect()
{
}
#pragma endregion



#pragma region BuildShadowMapEffect
BuildShadowMapEffect::BuildShadowMapEffect(ID3D11Device* device, const std::wstring& filename)
: DXEffect(device, filename)
{
	BuildShadowMapTech           = mFX->GetTechniqueByName("BuildShadowMapTech");
	BuildShadowMapAlphaClipTech  = mFX->GetTechniqueByName("BuildShadowMapAlphaClipTech");

	BuildShadowMapSkinnedTech          = mFX->GetTechniqueByName("BuildShadowMapSkinnedTech");
	BuildShadowMapAlphaClipSkinnedTech = mFX->GetTechniqueByName("BuildShadowMapAlphaClipSkinnedTech");

	TessBuildShadowMapTech           = mFX->GetTechniqueByName("TessBuildShadowMapTech");
	TessBuildShadowMapAlphaClipTech  = mFX->GetTechniqueByName("TessBuildShadowMapAlphaClipTech");
	
	ViewProj          = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	BoneTransforms    = mFX->GetVariableByName("gBoneTransforms")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	HeightScale       = mFX->GetVariableByName("gHeightScale")->AsScalar();
	MaxTessDistance   = mFX->GetVariableByName("gMaxTessDistance")->AsScalar();
	MinTessDistance   = mFX->GetVariableByName("gMinTessDistance")->AsScalar();
	MinTessFactor     = mFX->GetVariableByName("gMinTessFactor")->AsScalar();
	MaxTessFactor     = mFX->GetVariableByName("gMaxTessFactor")->AsScalar();
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	NormalMap         = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
}

BuildShadowMapEffect::~BuildShadowMapEffect()
{
}
#pragma endregion










#pragma region ClearGBufferEffect
ClearGBufferEffect::ClearGBufferEffect(ID3D11Device* device, const std::wstring& filename)
: DXEffect(device, filename)
{
	ClearTech  = mFX->GetTechniqueByName("ClearTech");
}

ClearGBufferEffect::~ClearGBufferEffect()
{
}
#pragma endregion

#pragma region ShadowMapEffect
ShadowMapEffect::ShadowMapEffect(ID3D11Device* device, const std::wstring& filename)
: DXEffect(device, filename)
{
	BasicShadowDirTech		= mFX->GetTechniqueByName("BasicShadowDir");
	BasicShadowPointTech	= mFX->GetTechniqueByName("BasicShadowPoint");
	BasicShadowSpotTech		= mFX->GetTechniqueByName("BasicShadowSpot");
	AlphaClipShadowDirTech	= mFX->GetTechniqueByName("AlphaClipShadowDir");
	AlphaClipShadowPointTech= mFX->GetTechniqueByName("AlphaClipShadowPoint");
	AlphaClipShadowSpotTech	= mFX->GetTechniqueByName("AlphaClipShadowSpot");

	SkinnedBasicShadowDirTech		= mFX->GetTechniqueByName("SkinnedBasicShadowDir");
	SkinnedBasicShadowPointTech		= mFX->GetTechniqueByName("SkinnedBasicShadowPoint");
	SkinnedBasicShadowSpotTech		= mFX->GetTechniqueByName("SkinnedBasicShadowSpot");
	SkinnedAlphaClipShadowDirTech	= mFX->GetTechniqueByName("SkinnedAlphaClipShadowDir");
	SkinnedAlphaClipShadowPointTech	= mFX->GetTechniqueByName("SkinnedAlphaClipShadowPoint");
	SkinnedAlphaClipShadowSpotTech	= mFX->GetTechniqueByName("SkinnedAlphaClipShadowSpot");

	DiffuseMap  = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	TexTransform	= mFX->GetVariableByName("gTexTransform")->AsMatrix();
	WorldViewProj	= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	BoneTransforms	= mFX->GetVariableByName("gBoneTransforms")->AsMatrix();
}

ShadowMapEffect::~ShadowMapEffect()
{
}
#pragma endregion

#pragma region CombineFinalEffect
CombineFinalEffect::CombineFinalEffect(ID3D11Device* device, const std::wstring& filename)
: DXEffect(device, filename)
{
	MonoTech  = mFX->GetTechniqueByName("Mono");
	ColorTech  = mFX->GetTechniqueByName("Color");
	TransparencyColorTech  = mFX->GetTechniqueByName("TransparencyColor");
	AlphaTransparencyColorTech  = mFX->GetTechniqueByName("AlphaTransparencyColor");
	AlphaClipColorTech  = mFX->GetTechniqueByName("AlphaClipColor");

	Opacity = mFX->GetVariableByName("g_Opacity")->AsScalar();

	Texture = mFX->GetVariableByName("g_Texture")->AsShaderResource();
}

CombineFinalEffect::~CombineFinalEffect()
{
}
#pragma endregion

#pragma region DXEffects

BasicEffect*		   DXEffects::BasicFX = 0;
BuildShadowMapEffect*  DXEffects::BuildShadowMapFX = 0;
ClearGBufferEffect*    DXEffects::ClearGBufferFX = 0;
CombineFinalEffect*	   DXEffects::CombineFinalFX = 0;
ObjectDeferredEffect*  DXEffects::ObjectDeferredFX = 0;
TiledLightningEffect*  DXEffects::TiledLightningFX = 0;
ShadowMapEffect*	   DXEffects::ShadowMapFX = 0;

void DXEffects::InitAll(ID3D11Device* device)
{
	//BasicFX			  = new BasicEffect(device, L"Graphics/DirectX/Shaders/Basic.fxo");
	BuildShadowMapFX  = new BuildShadowMapEffect(device, L"Graphics/DirectX/Shaders/BuildShadowMap.fxo");
	ClearGBufferFX    = new ClearGBufferEffect(device, L"Graphics/DirectX/Shaders/ClearGBuffer.fxo");
	CombineFinalFX    = new CombineFinalEffect(device, L"Graphics/DirectX/Shaders/CombineFinal.fxo");
	ObjectDeferredFX  = new ObjectDeferredEffect(device, L"Graphics/DirectX/Shaders/ObjectDeferred.fxo");
	TiledLightningFX  = new TiledLightningEffect(device, L"Graphics/DirectX/Shaders/TiledLightning.fxo");
	ShadowMapFX		  = new ShadowMapEffect(device, L"Graphics/DirectX/Shaders/ShadowMap.fxo");
}

void DXEffects::DestroyAll()
{
	SafeDelete(BasicFX);
	SafeDelete(BuildShadowMapFX);
	SafeDelete(ClearGBufferFX);
	SafeDelete(CombineFinalFX);
	SafeDelete(ObjectDeferredFX);
	SafeDelete(TiledLightningFX);
	SafeDelete(ShadowMapFX);
}

#pragma endregion