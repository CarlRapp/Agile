#pragma once
#ifndef DXVERTEX_H
#define DXVERTEX_H

#include "DXEffects.h"
#include <D3D11.h>
#include <DirectXMath.h>


#include <d3dCompiler.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <DirectXMath.h>

#include <d3dx11effect.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "Effects11D.lib")

namespace DXVertex
{
	const int NUMBER_OF_BONES_PER_VERTEX = 4;

	struct Quad
	{
		DirectX::XMFLOAT2 Pos;
		DirectX::XMFLOAT2 Tex;
	};

	struct PosTex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};

	// Basic 32-byte vertex structure.
	struct Basic32
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
	};

};

class DXInputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC Quad[2];
	static const D3D11_INPUT_ELEMENT_DESC PosTex[2];
	static const D3D11_INPUT_ELEMENT_DESC Basic32[3];
};

class DXInputLayouts
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11InputLayout* Pos;
	static ID3D11InputLayout* Quad;
	static ID3D11InputLayout* PosTex;
	static ID3D11InputLayout* Basic32;
};

#endif