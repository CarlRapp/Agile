#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "../../stdafx.h"
#include "DXMesh.h"
#include "DXTextureManager.h"
#include "DXVertex.h"
#include "../../Storage/ModelData.h"
#include <DirectXCollision.h>
#include "DXLightHelper.h"

//#include "ModelLoader.h"

using namespace std;

class DXModel
{
	vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
	vector<ID3D11ShaderResourceView*> NormalMapSRV;
public:
	DXModel(ID3D11Device* device, DXTextureManager& texMgr, ModelData* data);
	~DXModel(void);

	UINT SubsetCount;

	vector<DXMaterial> Mat;
	
	bool HasDiffuseMaps() { return !DiffuseMapSRV.empty(); }
	bool HasNormalMaps() { return !NormalMapSRV.empty(); }


	ID3D11ShaderResourceView* GetDiffuseMap(int Subset)
	{
		return DiffuseMapSRV[Subset];
	}

	ID3D11ShaderResourceView* GetNormalMap(int Subset)
	{
		return NormalMapSRV[Subset];
	}
	
	// Keep CPU copies of the mesh data to read from.  
	vector<DXVertex::PosNormalTexTan> Vertices;
	vector<UINT> Indices;
	vector<DXMesh::Subset> Subsets;
	
	/*
	// Keep CPU copies of the mesh data to read from.  
	std::vector<Vertex::PosNormalTexTan>* Vertices;
	std::vector<USHORT>* Indices;
	std::vector<Mesh::Subset>* Subsets;
	*/
	DXMesh ModelMesh;

	DirectX::BoundingSphere					m_BoundingSphere;
	DirectX::BoundingOrientedBox			m_BoundingOrientedBox;


};


struct ModelInstance
{
private:
	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

public:

	DXModel					*model;
	DirectX::XMFLOAT4X4		*world;
	DirectX::XMFLOAT4X4		*worldInverseTranspose;
	float					*explodeTime;


	ModelInstance()
	{
	}
};


#endif