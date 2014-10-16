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

#include "DXMeshManager.h"

using namespace std;

class DXModel
{
	vector<ID3D11ShaderResourceView*> DiffuseTexSRV;
	ID3D11ShaderResourceView* BlendTexSRV;
	vector<ID3D11ShaderResourceView*> NormalTexSRV;

	std::string m_modelName;
	std::string m_name;

public:
	DXModel(ID3D11Device* device, DXTextureManager& texMgr, ModelData* data, std::string _name, DXMeshManager& meshMgr);
	DXModel(DXModel* _parent, std::string suffix);
	~DXModel(void);

	UINT SubsetCount;

	vector<DXMaterial> Mat;
	
	bool HasDiffuseTextures() { return !DiffuseTexSRV.empty(); }
	bool HasNormalTextures() { return !NormalTexSRV.empty(); }


	ID3D11ShaderResourceView* GetDiffuseTexture(int Subset)
	{
		return DiffuseTexSRV[Subset];
	}

	ID3D11ShaderResourceView* GetBlendTexture()
	{
		return BlendTexSRV;
	}

	ID3D11ShaderResourceView* GetNormalTexture(int Subset)
	{
		return NormalTexSRV[Subset];
	}

	std::string GetModelName() { return m_modelName; }
	std::string GetName() { return m_name; }
	
	void SetBlendTexture(std::string _filename, DXTextureManager& texMgr);

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
	DXMesh* m_modelMesh;

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