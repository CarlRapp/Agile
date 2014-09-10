#pragma once
#ifndef DXMESH_H
#define DXMESH_H

#include "..\..\stdafx.h"

#include <d3d11.h>
#include <vector>
class DXMesh
{
public:
	struct Subset
	{
		Subset() : 
	      Id(-1), 
			VertexStart(0), VertexCount(0),
			FaceStart(0), FaceCount(0)
		{
		}

		UINT Id;
		UINT VertexStart;
		UINT VertexCount;
		UINT FaceStart;
		UINT FaceCount;
	};

public:
	DXMesh(void);
	~DXMesh(void);

	template <typename VertexType>
	void SetVertices(ID3D11Device* device, const VertexType* vertices, UINT count);

	void SetIndices(ID3D11Device* device, const UINT* indices, UINT count);

	void SetSubsetTable(std::vector<Subset>& subsetTable);

	void Draw(ID3D11DeviceContext* dc, UINT subsetId);

private:
	DXMesh(const DXMesh& rhs);
	DXMesh& operator=(const DXMesh& rhs);

private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	DXGI_FORMAT mIndexBufferFormat; // Always 16-bit
	UINT mVertexStride;

	std::vector<Subset> mSubsetTable;
};


template <typename VertexType>
void DXMesh::SetVertices(ID3D11Device* device, const VertexType* vertices, UINT count)
{
	ReleaseCOM(mVB);

	mVertexStride = sizeof(VertexType);

	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexType) * count;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;

    device->CreateBuffer(&vbd, &vinitData, &mVB);
}

#endif