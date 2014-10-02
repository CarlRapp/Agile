#pragma once
#ifndef DXTEXTUREMANAGER_H
#define DXTEXTUREMANAGER_H
#define UNICODE


#include "..\..\stdafx.h"
//#include <d3d11.h>


#include <D3D11.h>
#include <d3dx11tex.h>

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#endif

#include <map>


class DXTextureManager
{
private:
	DXTextureManager(const DXTextureManager& rhs);
	DXTextureManager& operator=(const DXTextureManager& rhs);

	ID3D11Device* md3dDevice;
	std::map<std::string, ID3D11ShaderResourceView*> mTextureSRV;

	ID3D11ShaderResourceView* m_RandomTex;

	void buildRandomTex(ID3D11Device *Device);

	float RandF();
	float RandF(float a, float b);

public:
	DXTextureManager();
	~DXTextureManager();

	void Init(ID3D11Device* device);

	ID3D11ShaderResourceView* CreateTexture(std::string filename);
	ID3D11ShaderResourceView* GetRandomTexture() { return m_RandomTex; }
};

#endif