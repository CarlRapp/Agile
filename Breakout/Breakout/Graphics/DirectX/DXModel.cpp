#include "DXModel.h"

DXModel::DXModel(ID3D11Device* device, DXTextureManager& texMgr, ModelData* data, std::string _name, DXMeshManager& meshMgr)
{
	m_name = _name;
	m_modelName = _name;
	//std::vector<MaterialLoader> mats;

	//ModelLoader loader;
	//loader.Load(modelFilename, Vertices, Indices, Subsets, mats, SkinnedData);

	//DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYaw(0, PI, 0);
	
	int index = 0;
	int id = 0;
	for (Group* group : data->Groups)
	{
		DXMesh::Subset subset;
		subset.m_id = id;
		subset.m_faceStart = index / 3;
		subset.m_vertexStart = index;
		subset.m_faceCount = group->triangles->size();
		subset.m_vertexCount = subset.m_faceCount * 3;

		for (Triangle triangle : *(group->triangles))
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
				vertex.pos.z *= -1;

				//DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&triangle.Vertices[i].Position);
				//pos = DirectX::XMVector3TransformCoord(pos, rot);
				//DirectX::XMStoreFloat3(&vertex.pos, pos);

				vertex.normal = triangle.Vertices[i].Normal;
				vertex.normal.z *= -1;

				//DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&triangle.Vertices[i].Normal);
				//normal = DirectX::XMVector3TransformCoord(normal, rot);
				//DirectX::XMStoreFloat3(&vertex.normal, normal);

				vertex.tex = triangle.Vertices[i].Texture;
				vertex.tex.y = 1.0f - vertex.tex.y;

				Vertices.push_back(vertex);
				Indices.push_back(index);
				index++;
			}
		}
		
		Subsets.push_back(subset);

		id++;
	}

	m_modelMesh = new DXMesh();
	m_modelMesh->SetVertices(device, &Vertices[0], Vertices.size());
	m_modelMesh->SetIndices(device, &Indices[0], Indices.size());
	m_modelMesh->SetSubsetTable(Subsets);

	meshMgr.AddMesh(_name, m_modelMesh);

	SubsetCount = Subsets.size();

	
	//SubsetCount = mats.size();

	//TPM = mats[0].DiffuseMapNames.size();
	for(UINT i = 0; i < SubsetCount; ++i)
	{
		DXMaterial m;

		BlendTexSRV = texMgr.CreateTexture("emptyBlend.png");

		if (data->Groups[i]->material)
		{
			if (data->Groups[i]->material->Map_Kd != "none")
			{
				ID3D11ShaderResourceView* diffuseTexSRV = texMgr.CreateTexture(data->Groups[i]->material->Map_Kd);
				DiffuseTexSRV.push_back(diffuseTexSRV);
			}

			if (data->Groups[i]->material->Map_bump != "none")
			{
				ID3D11ShaderResourceView* normalTexSRV = texMgr.CreateTexture(data->Groups[i]->material->Map_bump);
				NormalTexSRV.push_back(normalTexSRV);
			}

			m.SpecPower = data->Groups[i]->material->Ns;
			m.SpecIntensity = data->Groups[i]->material->Ks[0];
		}
		else
		{
			m.SpecPower = 1.0f;
			m.SpecIntensity = 0.0f;
		}
		

		Mat.push_back(m);

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

DXModel::DXModel(DXModel* _parent, std::string suffix)
{
	BlendTexSRV = _parent->BlendTexSRV;
	DiffuseTexSRV = _parent->DiffuseTexSRV;
	Indices = _parent->Indices;
	Mat = _parent->Mat;
	m_modelMesh = _parent->m_modelMesh;
	m_BoundingOrientedBox = _parent->m_BoundingOrientedBox;
	m_BoundingSphere = _parent->m_BoundingSphere;
	SubsetCount = _parent->SubsetCount;
	Subsets = _parent->Subsets;
	Vertices = _parent->Vertices;
	m_modelName = _parent->m_modelName;
	m_name = m_modelName + suffix;
}

DXModel::~DXModel(void)
{

}

void DXModel::SetBlendTexture(std::string _filename, DXTextureManager& texMgr)
{
	ID3D11ShaderResourceView* blendTex = texMgr.CreateTexture(_filename);

	if (blendTex)
		BlendTexSRV = blendTex;
}

