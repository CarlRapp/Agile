#pragma once
#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include "..\..\stdafx.h"
#include <DirectXCollision.h>
#include <vector>

#define DIRECTION_MAXRANGE 10000.0f
// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

static const DirectX::XMFLOAT2 SHADOWMAP_512 = DirectX::XMFLOAT2(512, 512);
static const DirectX::XMFLOAT2 SHADOWMAP_1024 = DirectX::XMFLOAT2(1024, 1024);
static const DirectX::XMFLOAT2 SHADOWMAP_2048 = DirectX::XMFLOAT2(2048, 2048);
static const DirectX::XMFLOAT2 SHADOWMAP_4096 = DirectX::XMFLOAT2(4096, 4096);

static int ShadowTileSize(DirectX::XMFLOAT2 resolution)
{
	if (resolution.x == SHADOWMAP_512.x && resolution.y == SHADOWMAP_512.y)
		return 1;

	else if (resolution.x == SHADOWMAP_1024.x && resolution.y == SHADOWMAP_1024.y)
		return 4;

	else if (resolution.x == SHADOWMAP_2048.x && resolution.y == SHADOWMAP_2048.y)
		return 16;

	else if (resolution.x == SHADOWMAP_4096.x && resolution.y == SHADOWMAP_4096.y)
		return 64;

	else return 0;
}


struct GPULight
{
	GPULight(void) { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT3 Color;
	float pad0;
};

struct GPUDirectionalLight : GPULight
{
	GPUDirectionalLight(void) { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT4 Direction;
	//shadow info	
	UINT	 ShadowIndex[4];
	DirectX::XMFLOAT2 Resolution;
	bool	 HasShadow;
};

struct GPUPointLight : GPULight
{
	GPUPointLight(void) { ZeroMemory(this, sizeof(this)); }
	// Packed into 4D vector: (Position, Range)
	DirectX::XMFLOAT3 Position;
	float Range;

	//DirectX::XMFLOAT3 Intensity;
	//float pad1;

	//shadow info	
	UINT	 ShadowIndex[6];
	DirectX::XMFLOAT2 Resolution;
	bool	 HasShadow;
};

struct GPUSpotLight : GPULight
{
	DirectX::XMFLOAT3 Position;
	float Range;
	// Packed into 4D vector: (Direction, Spot)
	DirectX::XMFLOAT3 Direction;
	float angle;
	//shadow info
	DirectX::XMFLOAT2 Resolution;
	UINT	 ShadowIndex;
	bool	 HasShadow;
};


class Light
{
protected:
public:
	DirectX::XMFLOAT3 *Color;
	DirectX::XMFLOAT3 *Intensity;

	Light(void) { }
	virtual ~Light(){ }
	//GPULight* GetGPULight() { return gpuLight; }
};

class DirectionalLight : public Light
{
	GPUDirectionalLight* gpuLight;
public:
	DirectionalLight(void) { gpuLight = new GPUDirectionalLight(); }

	DirectX::XMMATRIX GetViewMatrix(DirectX::BoundingFrustum& frustum, float offset);
	DirectX::XMMATRIX GetProjectionMatrix(/*float nearZ, float farZ*/);

	void GetViewProjOBB(DirectX::BoundingFrustum& frustum, float offset, DirectX::XMFLOAT4X4& View, DirectX::XMFLOAT4X4& Proj, DirectX::BoundingOrientedBox& OBB);

	GPUDirectionalLight* GetGPULight() 
	{ 
		return gpuLight; 
	}
	/*
	BoundingFrustum GetBoundingOrientedBox(BoundingFrustum& frustum, float offset)
	{
		return MathHelper::GenerateBoundingFrustum(GetViewMatrix(frustum, offset), GetProjectionMatrix(0.0f, DIRECTION_MAXRANGE));
		//return BoundingFrustum(Position, Range);
	}
	*/
	
};

class PointLight : public Light
{
	GPUPointLight* gpuLight;
public:

	DirectX::XMFLOAT3 *Position;
	float *Range;

	PointLight(void) { gpuLight = new GPUPointLight(); }

	std::vector<DirectX::XMFLOAT4X4> GetViewMatrixes();
	DirectX::XMMATRIX GetProjectionMatrix(/*float nearZ, float farZ*/);
	
	//std::vector<DirectX::BoundingFrustum> GetBoundingFrustums()
	//{
	//	std::vector<DirectX::BoundingFrustum> frustums;
	//	
	//	std::vector<DirectX::XMFLOAT4X4> Views = GetViewMatrixes();;
	//	DirectX::XMMATRIX Proj = GetProjectionMatrix(/*0.0f, Range*/);
	//	
	//	for (int i = 0; i < Views.size(); ++i)
	//	{
	//		DirectX::BoundingFrustum frustum = MathHelper::GenerateBoundingFrustum(XMLoadFloat4x4(&Views[i]), Proj);
	//		frustums.push_back(frustum);
	//	}		
	//	return frustums;
	//}


	DirectX::BoundingSphere GetBoundingSphere()
	{
		return DirectX::BoundingSphere(gpuLight->Position, gpuLight->Range);
	}

	GPUPointLight* GetGPULight()
	{ 
		gpuLight->Color = *Color;
		gpuLight->Position = *Position;
		gpuLight->Position.z *= -1;
		gpuLight->Range = *Range;
		return gpuLight; 
	}
};

class SpotLight : public Light
{
	GPUSpotLight* gpuLight;
public:
	SpotLight(void) { gpuLight = new GPUSpotLight(); }

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix(/*float nearZ, float farZ*/);

	//DirectX::BoundingFrustum GetBoundingFrustum()
	//{
	//	return MathHelper::GenerateBoundingFrustum(GetViewMatrix(), GetProjectionMatrix(/*0.0f, Range*/));
	//}

	virtual GPUSpotLight* GetGPULight() { return gpuLight; }
};

struct DXMaterial
{
	DXMaterial() { ZeroMemory(this, sizeof(this)); }

	float SpecIntensity;
	float SpecPower;
	DirectX::XMFLOAT2 pad;
};


#endif