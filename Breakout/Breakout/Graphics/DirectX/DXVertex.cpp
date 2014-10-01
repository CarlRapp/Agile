#include "DXVertex.h"


#pragma region DXInputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::m_quad[2] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::m_posTex[2] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

/*
const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::Basic32[3] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
*/
const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::m_posNormalTexTan[4] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

/*
const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::m_posNormalTexTanCol[5] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
*/

const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::m_instancedPosNormalTexTan[9] =
{
	//Vertex buffer
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	//Instance buffer
	{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "EXPLODETIME", 0, DXGI_FORMAT_R32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};

#pragma endregion

#pragma region InputLayouts

ID3D11InputLayout* DXInputLayouts::m_pos = 0;
ID3D11InputLayout* DXInputLayouts::m_posTex = 0;
ID3D11InputLayout* DXInputLayouts::m_posNormalTexTan = 0;
//ID3D11InputLayout* DXInputLayouts::m_posNormalTexTanCol = 0;
ID3D11InputLayout* DXInputLayouts::m_quad = 0;
//ID3D11InputLayout* DXInputLayouts::m_basic32 = 0;
ID3D11InputLayout* DXInputLayouts::m_instancedPosNormalTexTan = 0;

void DXInputLayouts::InitAll(ID3D11Device* _device)
{
	D3DX11_PASS_DESC passDesc;
	
	//
	// Pos
	//
	
	//
	// PosTex
	//

	DXEffects::m_shadowMapFX->m_basicShadowDirTech->GetPassByIndex(0)->GetDesc(&passDesc);
	_device->CreateInputLayout(DXInputLayoutDesc::m_posTex, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_posTex);

	//
	// Basic32
	//

	//DXEffects::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	//_device->CreateInputLayout(DXInputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_basic32);


	//
	// NormalMap
	//

	DXEffects::m_objectDeferredFX->m_texNormalTech->GetPassByIndex(0)->GetDesc(&passDesc);
	_device->CreateInputLayout(DXInputLayoutDesc::m_posNormalTexTan, 4, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_posNormalTexTan);


	DXEffects::m_objectDeferredFX->m_basicInstancedTech->GetPassByIndex(0)->GetDesc(&passDesc);
	_device->CreateInputLayout(DXInputLayoutDesc::m_instancedPosNormalTexTan, 9, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_instancedPosNormalTexTan);


	//DXEffects::m_objectDeferredFX->m_texNormalTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//_device->CreateInputLayout(DXInputLayoutDesc::m_posNormalTexTanCol, 5, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_posNormalTexTanCol);


	//
	// CombineFinal
	//

	DXEffects::m_combineFinalFX->m_colorTech->GetPassByIndex(0)->GetDesc(&passDesc);
	_device->CreateInputLayout(DXInputLayoutDesc::m_quad, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_quad);

}

void DXInputLayouts::DestroyAll()
{
	ReleaseCOM(m_pos);
	ReleaseCOM(m_posTex);
	//ReleaseCOM(m_basic32);
	ReleaseCOM(m_posNormalTexTan);
	//ReleaseCOM(m_posNormalTexTanCol);
	ReleaseCOM(m_quad);

	ReleaseCOM(m_instancedPosNormalTexTan);
}

#pragma endregion
