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

	ReleaseCOM(m_RandomTex);
}

void DXTextureManager::Init(ID3D11Device* device)
{
	md3dDevice = device;
	buildRandomTex(device);
}

ID3D11ShaderResourceView* DXTextureManager::CreateTexture(std::string filename)
{
	ID3D11ShaderResourceView* srv = 0;

	filename = TEXTURE_ROOT + filename;

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
 

//skapar en textur med slumpmässigt innehåll.
void DXTextureManager::buildRandomTex(ID3D11Device *Device)
{
	//
	// Create the random data.
	//
	DirectX::XMFLOAT4 randomValues[1024];
	for (int i = 0; i < 1024; ++i)
	{
		//sätter slumpmässig data för R, G, B och A i varje texel.
		randomValues[i].x = RandF(-1.0f, 1.0f);
		randomValues[i].y = RandF(-1.0f, 1.0f);
		randomValues[i].z = RandF(-1.0f, 1.0f);
		randomValues[i].w = RandF(-1.0f, 1.0f);
	}
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024 * sizeof(DirectX::XMFLOAT4);
	initData.SysMemSlicePitch = 1024 * sizeof(DirectX::XMFLOAT4);
	//
	// Create the texture.
	//
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex = 0;
	HRESULT(Device->CreateTexture1D(&texDesc, &initData, &randomTex));
	//
	// Create the resource view.
	//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	HRESULT(Device->CreateShaderResourceView(randomTex, &viewDesc, &m_RandomTex));

	ReleaseCOM(randomTex);
}

// Returns random float in [0, 1).
float DXTextureManager::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}
// Returns random float in [a, b).
float DXTextureManager::RandF(float a, float b)
{
	return a + RandF()*(b - a);
}
