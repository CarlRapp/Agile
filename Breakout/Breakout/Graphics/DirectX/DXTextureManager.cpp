#include "DXTextureManager.h"

DXTextureManager::DXTextureManager() : md3dDevice(0)
{
}

DXTextureManager::~DXTextureManager()
{
	for(auto it = mTextureSRV.begin(); it != mTextureSRV.end(); ++it)
    {
		ReleaseCOM(it->second);
    }

	mTextureSRV.clear();
}

void DXTextureManager::Init(ID3D11Device* device)
{
	md3dDevice = device;
}

ID3D11ShaderResourceView* DXTextureManager::CreateTexture(std::string filename)
{
	ID3D11ShaderResourceView* srv = 0;

	// Does it already exist?
	if( mTextureSRV.find(filename) != mTextureSRV.end() )
	{
		srv = mTextureSRV[filename];
	}
	else
	{
		std::wstring widestr = std::wstring(filename.begin(), filename.end());
		D3DX11CreateShaderResourceViewFromFile(md3dDevice, widestr.c_str(), 0, 0, &srv, 0 );

		mTextureSRV[filename] = srv;
	}

	return srv;
}
 
