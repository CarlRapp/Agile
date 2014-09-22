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
	      m_id(-1), 
			m_vertexStart(0), m_vertexCount(0),
			m_faceStart(0), m_faceCount(0)
		{
		}

		UINT m_id;
		UINT m_vertexStart;
		UINT m_vertexCount;
		UINT m_faceStart;
		UINT m_faceCount;
	};

public:
	DXMesh(void);
	~DXMesh(void);

	template <typename VertexType>
	void SetVertices(ID3D11Device* _device, const VertexType* _vertices, UINT _count);

	void SetIndices(ID3D11Device* _device, const UINT* _indices, UINT _count);

	void SetSubsetTable(std::vector<Subset>& _subsetTable);

	void Draw(ID3D11DeviceContext* _dc, UINT _subsetId);
	void DrawInstanced(ID3D11DeviceContext* _dc, UINT _subsetId, ID3D11Buffer *_ib, int _numInstances);

private:
	DXMesh(const DXMesh& _rhs);
	DXMesh& operator=(const DXMesh& _rhs);

private:
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	DXGI_FORMAT m_indexBufferFormat; // Always 16-bit
	UINT m_vertexStride;

	std::vector<Subset> m_subsetTable;
};


template <typename VertexType>
void DXMesh::SetVertices(ID3D11Device* _device, const VertexType* _vertices, UINT _count)
{
	ReleaseCOM(m_VB);

	m_vertexStride = sizeof(VertexType);

	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexType) * _count;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = _vertices;

	_device->CreateBuffer(&vbd, &vinitData, &m_VB);
}

#endif