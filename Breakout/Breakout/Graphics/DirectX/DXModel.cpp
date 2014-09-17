#include "DXModel.h"

DXModel::DXModel(ID3D11Device* device, DXTextureManager& texMgr, ModelData* data)
{
	//std::vector<MaterialLoader> mats;

	//ModelLoader loader;
	//loader.Load(modelFilename, Vertices, Indices, Subsets, mats, SkinnedData);

	int index = 0;
	int id = 0;
	for (Group* group : data->Groups)
	{
		DXMesh::Subset subset;
		subset.m_id = id;
		subset.m_faceStart = index / 3;
		subset.m_vertexStart = index;
		subset.m_faceCount = group->triangles.size();
		subset.m_vertexCount = subset.m_faceCount * 3;

		for (Triangle triangle : group->triangles)
		{
			for (int i = 0; i < 3; i++)
			{
				DXVertex::PosNormalTexTanCol vertex;
				vertex.color = DirectX::XMFLOAT3(1, 1, 1);
				vertex.tangentU = DirectX::XMFLOAT4(0, 0, 0, 0);
				memcpy(&vertex.pos, &triangle.Vertices[i].Position, sizeof(DirectX::XMFLOAT3));
				memcpy(&vertex.normal, &triangle.Vertices[i].Normal, sizeof(DirectX::XMFLOAT3));
				memcpy(&vertex.tex, &triangle.Vertices[i].Texture, sizeof(DirectX::XMFLOAT2));

				Vertices.push_back(vertex);
				Indices.push_back(index);
				index++;
			}
		}
		
		Subsets.push_back(subset);

		id++;
	}

	ModelMesh.SetVertices(device, &Vertices[0], Vertices.size());
	ModelMesh.SetIndices(device, &Indices[0], Indices.size());
	ModelMesh.SetSubsetTable(Subsets);

	SubsetCount = Subsets.size();

	
	//SubsetCount = mats.size();

	//TPM = mats[0].DiffuseMapNames.size();
	for(UINT i = 0; i < SubsetCount; ++i)
	{
		/*
		Mat.push_back(mats[i].Mat);

		for (int j = 0; j < mats[i].DiffuseMapNames.size(); ++j)
		{
			if (mats[i].DiffuseMapNames[j] != "")
			{
				ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(texturePath + mats[i].DiffuseMapNames[j]);
				if (diffuseMapSRV)
					DiffuseMapSRV.push_back(diffuseMapSRV);
			}
		}

		for (int j = 0; j < mats[i].NormalMapNames.size(); ++j)
		{
			if (mats[i].NormalMapNames[j] != "")
			{
				ID3D11ShaderResourceView* normalMapSRV = texMgr.CreateTexture(texturePath + mats[i].NormalMapNames[j]);
				if (normalMapSRV)
					NormalMapSRV.push_back(normalMapSRV);
			}
		}

		*/
		
	}
	/*
	DirectX::BoundingBox AABB;
	DirectX::BoundingBox::CreateFromPoints(AABB, Vertices.size(), &Vertices[0].pos, sizeof(DXVertex::PosNormalTexTanCol));

	DirectX::BoundingOrientedBox::CreateFromBoundingBox(m_BoundingOrientedBox, AABB);

	XMFLOAT3 Extents = m_BoundingOrientedBox.Extents;
	m_SmallestRadiusInBox = (Extents.x > Extents.z) ? Extents.z : Extents.x;

	BoundingSphere::CreateFromBoundingBox(m_BoundingSphere, m_BoundingOrientedBox);

	*/
	//BoundingOrientedBox::CreateFromPoints(m_BoundingOrientedBox, Vertices.size(), &Vertices[0].Pos, sizeof(Vertex::PosNormalTexTanSkinned));
}

DXModel::~DXModel(void)
{
/*
	KOMMENTERADE BORT DETTA D� DESSA
	TEXTURER REDAN FANNS I TEXTURE MANAGER
	OCH DET BLEV PROBLEM N�R TEXTURE MANAGERNS
	DESTRUKTOR K�RS OCH SEDAN DENNAS. (EFTERSOM
	TEXTUREN D� REDAN �R BORTTAGEN!)

	if ( DiffuseMapSRV.size() > 0 )
	{
		for ( int i = 0; i < DiffuseMapSRV.size(); ++i )
		{
			if ( DiffuseMapSRV.at(i) )
			{
				DiffuseMapSRV.at(i)->Release();
				DiffuseMapSRV.at(i)	=	0;
			}
		}
		DiffuseMapSRV.clear();
	}

	if ( NormalMapSRV.size() > 0 )
	{
		for ( int i = 0; i < NormalMapSRV.size(); ++i )
		{
			if ( NormalMapSRV.at(i) )
			{
				NormalMapSRV.at(i)->Release();
				NormalMapSRV.at(i)	=	0;
			}
		}
		NormalMapSRV.clear();
	}
	*/
}
