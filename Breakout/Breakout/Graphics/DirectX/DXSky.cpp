//***************************************************************************************
// Sky.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "DXSky.h"
#include <vector>

#include "DXCamera.h"
#include "DXVertex.h"
#include "DXEffects.h"


DXSky::DXSky(ID3D11Device* _device)
{
	GenerateSphear(_device);
}

DXSky::~DXSky()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
}

bool DXSky::Load(std::string _cubemapFilename, DXTextureManager* _texMgr)
{
	m_cubeMapSRV = _texMgr->CreateTexture("CubeMaps/" + _cubemapFilename + ".dds");

	return m_cubeMapSRV;
}

ID3D11ShaderResourceView* DXSky::CubeMapSRV()
{
	return m_cubeMapSRV;
}

void DXSky::Draw(ID3D11DeviceContext* _dc, ICamera* _camera)
{
	// center Sky about eye in world space
	DirectX::XMFLOAT3 eyePos = _camera->GetPosition();
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);


	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(_camera->GetView());
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(_camera->GetProjection());

	DirectX::XMMATRIX VP = DirectX::XMMatrixMultiply(view, proj);

	DirectX::XMMATRIX WVP = XMMatrixMultiply(T, VP);

	DXEffects::m_skyFX->SetWorldViewProj(WVP);
	DXEffects::m_skyFX->SetCubeMap(m_cubeMapSRV);


	UINT stride = sizeof(DirectX::XMFLOAT3);
    UINT offset = 0;
    _dc->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	_dc->IASetIndexBuffer(m_IB, DXGI_FORMAT_R16_UINT, 0);
	_dc->IASetInputLayout(DXInputLayouts::m_pos);
	_dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc;
	DXEffects::m_skyFX->m_skyTech->GetDesc(&techDesc);

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		ID3DX11EffectPass* pass = DXEffects::m_skyFX->m_skyTech->GetPassByIndex(p);

		pass->Apply(0, _dc);

		_dc->DrawIndexed(m_indexCount, 0, 0);
	}
}

void DXSky::GenerateSphear(ID3D11Device* _device)
{
	float radius = 1;
	UINT stackCount = 30;
	UINT sliceCount = 30;

	std::vector<DXVertex::Pos> Vertices;
	std::vector<UINT> Indices;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	DXVertex::Pos topVertex;
	topVertex.pos = DirectX::XMFLOAT3(0.0f, +radius, 0.0f);

	DXVertex::Pos bottomVertex;
	bottomVertex.pos = DirectX::XMFLOAT3(0.0f, -radius, 0.0f);

	Vertices.push_back(topVertex);

	float phiStep = PI / stackCount;
	float thetaStep = 2.0f*PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			DXVertex::Pos v;

			// spherical to cartesian
			v.pos.x = radius*sinf(phi)*cosf(theta);
			v.pos.y = radius*cosf(phi);
			v.pos.z = radius*sinf(phi)*sinf(theta);

			Vertices.push_back(v);
		}
	}

	Vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		Indices.push_back(0);
		Indices.push_back(i + 1);
		Indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			Indices.push_back(baseIndex + i*ringVertexCount + j);
			Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)Vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		Indices.push_back(southPoleIndex);
		Indices.push_back(baseIndex + i);
		Indices.push_back(baseIndex + i + 1);
	}


	//Create Buffers

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(DXVertex::Pos) * Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &Vertices[0];

	_device->CreateBuffer(&vbd, &vinitData, &m_VB);


	m_indexCount = Indices.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * m_indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	std::vector<USHORT> indices16;
	indices16.assign(Indices.begin(), Indices.end());

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices16[0];

	_device->CreateBuffer(&ibd, &iinitData, &m_IB);
}