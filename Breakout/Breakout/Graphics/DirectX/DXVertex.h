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

	struct Pos
	{
		DirectX::XMFLOAT3 pos;
	};

	struct Quad
	{
		DirectX::XMFLOAT2 pos;
		DirectX::XMFLOAT2 tex;
	};

	struct PosTex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
	};
	/*
	// Basic 32-byte vertex structure.
	struct Basic32
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
	};
	*/
	struct PosNormalTexTan
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT4 tangentU;
	};
	/*
	struct PosNormalTexTanCol
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT4 tangentU;
		DirectX::XMFLOAT3 color;
	};
	*/
};

namespace DXInstance
{
	struct World
	{
		DirectX::XMFLOAT4X4 world;
		float ExplodeTime;
	};	
};

class DXInputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC m_pos[1];
	static const D3D11_INPUT_ELEMENT_DESC m_quad[2];
	static const D3D11_INPUT_ELEMENT_DESC m_posTex[2];
	//static const D3D11_INPUT_ELEMENT_DESC m_basic32[3];
	static const D3D11_INPUT_ELEMENT_DESC m_posNormalTexTan[4];
	//static const D3D11_INPUT_ELEMENT_DESC m_posNormalTexTanCol[5];

	static const D3D11_INPUT_ELEMENT_DESC m_instancedPosNormalTexTan[9];
};

class DXInputLayouts
{
public:
	static void InitAll(ID3D11Device* _device);
	static void DestroyAll();

	static ID3D11InputLayout* m_pos;
	static ID3D11InputLayout* m_quad;
	static ID3D11InputLayout* m_posTex;
	//static ID3D11InputLayout* Basic32;
	static ID3D11InputLayout* m_posNormalTexTan;
	//static ID3D11InputLayout* m_posNormalTexTanCol;

	static ID3D11InputLayout* m_instancedPosNormalTexTan;
};

#endif