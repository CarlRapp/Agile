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
				DXVertex::PosNormalTexTan vertex;
				//vertex.color = DirectX::XMFLOAT3(1, 1, 1);
				//vertex.tangentU = DirectX::XMFLOAT4(0, 0, 0, 0);
				//memcpy(&vertex.pos, &triangle.Vertices[i].Position, sizeof(DirectX::XMFLOAT3));
				//memcpy(&vertex.normal, &triangle.Vertices[i].Normal, sizeof(DirectX::XMFLOAT3));
				//memcpy(&vertex.tex, &triangle.Vertices[i].Texture, sizeof(DirectX::XMFLOAT2));

				vertex.tangentU = triangle.Vertices[i].Tangent;
				vertex.pos = triangle.Vertices[i].Position;
				vertex.normal = triangle.Vertices[i].Normal;
				vertex.tex = triangle.Vertices[i].Texture;

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
		if (data->Groups[i]->material)
		{
			if (data->Groups[i]->material->Map_Kd != "")
			{
				ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(data->Groups[i]->material->Map_Kd);
				DiffuseMapSRV.push_back(diffuseMapSRV);
			}


			if (data->Groups[i]->material->Map_bump != "")
			{
				ID3D11ShaderResourceView* normalMapSRV = texMgr.CreateTexture(data->Groups[i]->material->Map_bump);
				NormalMapSRV.push_back(normalMapSRV);
			}
		}
		


		

		//Mat.push_back(mats[i].Mat);

		//for (int j = 0; j < mats[i].DiffuseMapNames.size(); ++j)
		//{
		//	if (mats[i].DiffuseMapNames[j] != "")
		//	{
		//		ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(texturePath + mats[i].DiffuseMapNames[j]);
		//		if (diffuseMapSRV)
		//			DiffuseMapSRV.push_back(diffuseMapSRV);
		//	}
		//}

		//for (int j = 0; j < mats[i].NormalMapNames.size(); ++j)
		//{
		//	if (mats[i].NormalMapNames[j] != "")
		//	{
		//		ID3D11ShaderResourceView* normalMapSRV = texMgr.CreateTexture(texturePath + mats[i].NormalMapNames[j]);
		//		if (normalMapSRV)
		//			NormalMapSRV.push_back(normalMapSRV);
		//	}
		//}

		
		
	}
	
	DirectX::BoundingBox AABB;
	DirectX::BoundingBox::CreateFromPoints(AABB, Vertices.size(), &Vertices[0].pos, sizeof(DXVertex::PosNormalTexTan));

	DirectX::BoundingOrientedBox::CreateFromBoundingBox(m_BoundingOrientedBox, AABB);

	//DirectX::BoundingSphere::CreateFromBoundingBox(m_BoundingSphere, m_BoundingOrientedBox);
	DirectX::BoundingSphere::CreateFromPoints(m_BoundingSphere, Vertices.size(), &Vertices[0].pos, sizeof(DXVertex::PosNormalTexTan));

	
	//BoundingOrientedBox::CreateFromPoints(m_BoundingOrientedBox, Vertices.size(), &Vertices[0].Pos, sizeof(Vertex::PosNormalTexTanSkinned));
}

DXModel::~DXModel(void)
{
/*
	KOMMENTERADE BORT DETTA DÅ DESSA
	TEXTURER REDAN FANNS I TEXTURE MANAGER
	OCH DET BLEV PROBLEM NÄR TEXTURE MANAGERNS
	DESTRUKTOR KÖRS OCH SEDAN DENNAS. (EFTERSOM
	TEXTUREN DÅ REDAN ÄR BORTTAGEN!)

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

