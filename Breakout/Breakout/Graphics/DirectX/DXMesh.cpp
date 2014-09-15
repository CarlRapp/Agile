#include "DXMesh.h"



DXMesh::DXMesh()
	: m_VB(0), m_IB(0),
	m_indexBufferFormat(DXGI_FORMAT_R32_UINT),
	m_vertexStride(0)
{
}

DXMesh::~DXMesh()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
}

void DXMesh::SetIndices(ID3D11Device* _device, const UINT* _indices, UINT _count)
{
	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * _count;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = _indices;

	_device->CreateBuffer(&ibd, &iinitData, &m_IB);
}

void DXMesh::SetSubsetTable(std::vector<Subset>& _subsetTable)
{
	m_subsetTable = _subsetTable;
}

void DXMesh::Draw(ID3D11DeviceContext* _dc, UINT _subsetId)
{
    UINT offset = 0;

	_dc->IASetVertexBuffers(0, 1, &m_VB, &m_vertexStride, &offset);
	_dc->IASetIndexBuffer(m_IB, m_indexBufferFormat, 0);
	
	//dc->Draw(

	_dc->DrawIndexed(
		m_subsetTable[_subsetId].m_faceCount * 3,
		m_subsetTable[_subsetId].m_faceStart * 3,
		0);

		
}