#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "../../stdafx.h"
#include "DXMesh.h"
#include "DXTextureManager.h"
#include "DXVertex.h"
#include "../../Storage/ModelData.h"

//#include "ModelLoader.h"

using namespace std;

class DXModel
{
	int TPM;
	vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
	vector<ID3D11ShaderResourceView*> NormalMapSRV;
public:
	DXModel(ID3D11Device* device, DXTextureManager& texMgr, ModelData* data);
	~DXModel(void);

	UINT SubsetCount;

	//vector<Material> Mat;
	
	bool HasDiffuseMaps() { return !DiffuseMapSRV.empty(); }
	bool HasNormalMaps() { return !NormalMapSRV.empty(); }

	int  GetTPM() { return TPM; }

	ID3D11ShaderResourceView* GetDiffuseMap(int Subset, int textureIndex)
	{
		int index = TPM * Subset + textureIndex;
		return DiffuseMapSRV[index];
	}

	ID3D11ShaderResourceView* GetNormalMap(int Subset, int textureIndex)
	{
		int index = TPM * Subset + textureIndex;
		return NormalMapSRV[index];
	}
	
	// Keep CPU copies of the mesh data to read from.  
	vector<DXVertex::PosNormalTexTanCol> Vertices;
	vector<UINT> Indices;
	vector<DXMesh::Subset> Subsets;
	
	/*
	// Keep CPU copies of the mesh data to read from.  
	std::vector<Vertex::PosNormalTexTan>* Vertices;
	std::vector<USHORT>* Indices;
	std::vector<Mesh::Subset>* Subsets;
	*/
	DXMesh ModelMesh;

	//BoundingSphere					m_BoundingSphere;
	//BoundingOrientedBox				m_BoundingOrientedBox;

	float							m_SmallestRadiusInBox;


};

#define UPDATE_INTERVAL 1.0f / 50.0f
class ModelInstance
{
private:
	DXModel			*m_Model;
	Float4x4		m_World;
	float			m_Scale;


	int				m_TextureIndex;

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

public:



	
	Vector4						m_Rotation;
	Vector3						m_Translation;
	Float4x4				m_WorldInverseTranspose;

	

	void SetModel(DXModel* Model)
	{
		m_Model = Model;
	}

	DXModel* GetModel() { return m_Model; }

	void SetWorld(Float4x4& World, Float4x4& WorldInvTrans, float& Scale)
	{
		m_World = World;
		m_WorldInverseTranspose = WorldInvTrans;
		m_Scale = Scale;
	}

	void SetTextureIndex(int index) 
	{ 
		m_TextureIndex = Clamp<int>(index, 0, m_Model->GetTPM() - 1); 
	}

	int GetTextureIndex() { return m_TextureIndex; }

	Float4x4 GetWorld() { return m_World; }


	ModelInstance()
	{
		m_Model					=	NULL;
		m_World					=	Float4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		m_Scale					=	1;
		m_TextureIndex			=	0;
		m_WorldInverseTranspose =   Float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	}


	/*
	BoundingSphere GetBoundingSphere()
	{
		BoundingSphere out;
		if (m_Model)
		{
			XMMATRIX world = XMLoadFloat4x4(&m_World);
			m_Model->m_BoundingSphere.Transform(out, world);
		}
		return out;
	}

	BoundingOrientedBox GetBoundingOrientedBox()
	{
		BoundingOrientedBox out;
		if (m_Model)
		{			
			XMVECTOR rot	= XMLoadFloat4(&m_Rotation);
			XMVECTOR trans	= XMLoadFloat3(&m_Translation);
			out	= m_Model->m_BoundingOrientedBox;				
			out.Transform(out, m_Scale, rot, trans);
		}
		return out;
	}

	float GetSmallestRaduisInBox()
	{
		return m_Model->m_SmallestRadiusInBox * m_Scale;
	}
	*/
};


#endif