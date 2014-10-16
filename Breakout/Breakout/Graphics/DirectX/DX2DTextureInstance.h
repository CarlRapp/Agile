#pragma once
#ifndef DX2DTEXTUREINSTANCE_H
#define DX2DTEXTUREINSTANCE_H

#include <D3D11.h>
#include <d3dx11tex.h>

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#endif

struct DX2DTextureInstance
{
private:

public:

	ID3D11ShaderResourceView *Texture;
	float *X; 
	float *Y; 
	float *Width; 
	float *Height;

	DX2DTextureInstance()
	{
	}
};


#endif