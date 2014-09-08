#include "DXVertex.h"


#pragma region DXInputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::Quad[2] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::PosTex[2] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC DXInputLayoutDesc::Basic32[3] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

#pragma endregion

#pragma region InputLayouts

ID3D11InputLayout* DXInputLayouts::Pos = 0;
ID3D11InputLayout* DXInputLayouts::PosTex = 0;
ID3D11InputLayout* DXInputLayouts::Quad = 0;
ID3D11InputLayout* DXInputLayouts::Basic32 = 0;

void DXInputLayouts::InitAll(ID3D11Device* device)
{
	D3DX11_PASS_DESC passDesc;
	
	//
	// Pos
	//
	
	//
	// PosTex
	//

	Effects::ShadowMapFX->BasicShadowDirTech->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(DXInputLayoutDesc::PosTex, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PosTex);

	//
	// Basic32
	//

	//Effects::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(DXInputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &Basic32);
	
}

void DXInputLayouts::DestroyAll()
{
	ReleaseCOM(Pos);
	ReleaseCOM(PosTex);
	ReleaseCOM(Basic32);
	ReleaseCOM(Quad);
}

#pragma endregion
