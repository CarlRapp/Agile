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

	for (auto it = mLetterSRV.begin(); it != mLetterSRV.end(); ++it)
	{
		ReleaseCOM(it->second);
	}

	mTextureSRV.clear();
	mLetterSRV.clear();

	ReleaseCOM(m_RandomTex);
}

void DXTextureManager::Init(ID3D11Device* _device)
{
	md3dDevice = _device;
	buildRandomTex(_device);
	InitLetters(_device);
}

void DXTextureManager::InitLetters(ID3D11Device* _device)
{
	AddSymbolTexture(_device, 'A', DXText::A);
	AddSymbolTexture(_device, 'B', DXText::B);
	AddSymbolTexture(_device, 'C', DXText::C);
	AddSymbolTexture(_device, 'D', DXText::D);
	AddSymbolTexture(_device, 'E', DXText::E);
	AddSymbolTexture(_device, 'F', DXText::F);
	AddSymbolTexture(_device, 'G', DXText::G);
	AddSymbolTexture(_device, 'H', DXText::H);
	AddSymbolTexture(_device, 'I', DXText::I);
	AddSymbolTexture(_device, 'J', DXText::J);
	AddSymbolTexture(_device, 'K', DXText::K);
	AddSymbolTexture(_device, 'L', DXText::L);
	AddSymbolTexture(_device, 'M', DXText::M);
	AddSymbolTexture(_device, 'N', DXText::N);
	AddSymbolTexture(_device, 'O', DXText::O);
	AddSymbolTexture(_device, 'P', DXText::P);
	AddSymbolTexture(_device, 'Q', DXText::Q);
	AddSymbolTexture(_device, 'R', DXText::R);
	AddSymbolTexture(_device, 'S', DXText::S);
	AddSymbolTexture(_device, 'T', DXText::T);
	AddSymbolTexture(_device, 'U', DXText::U);
	AddSymbolTexture(_device, 'V', DXText::V);
	AddSymbolTexture(_device, 'W', DXText::W);
	AddSymbolTexture(_device, 'X', DXText::X);
	AddSymbolTexture(_device, 'Y', DXText::Y);
	AddSymbolTexture(_device, 'Z', DXText::Z);

	AddSymbolTexture(_device, '1', DXText::_1);
	AddSymbolTexture(_device, '2', DXText::_2);
	AddSymbolTexture(_device, '3', DXText::_3);
	AddSymbolTexture(_device, '4', DXText::_4);
	AddSymbolTexture(_device, '5', DXText::_5);
	AddSymbolTexture(_device, '6', DXText::_6);
	AddSymbolTexture(_device, '7', DXText::_7);
	AddSymbolTexture(_device, '8', DXText::_8);
	AddSymbolTexture(_device, '9', DXText::_9);
	AddSymbolTexture(_device, '0', DXText::_0);

	//AddSymbolTexture(_device, ':', DXText::A);
	//AddSymbolTexture(_device, ';', DXText::A);
	//AddSymbolTexture(_device, '<', DXText::A);
	//AddSymbolTexture(_device, '=', DXText::A);
	//AddSymbolTexture(_device, '>', DXText::A);
	//AddSymbolTexture(_device, '?', DXText::A);
	//AddSymbolTexture(_device, '@', DXText::A);
	//AddSymbolTexture(_device, ' ', DXText::A);
	//AddSymbolTexture(_device, '!', DXText::A);
	//AddSymbolTexture(_device, '"', DXText::A);
	//AddSymbolTexture(_device, '#', DXText::A);
	//AddSymbolTexture(_device, '$', DXText::A);
	//AddSymbolTexture(_device, '%', DXText::A);
	//AddSymbolTexture(_device, '&', DXText::A);
	//AddSymbolTexture(_device, '´', DXText::A);
	//AddSymbolTexture(_device, '(', DXText::A);
	//AddSymbolTexture(_device, ')', DXText::A);
	//AddSymbolTexture(_device, '*', DXText::A);
	//AddSymbolTexture(_device, '+', DXText::A);
	//AddSymbolTexture(_device, ',', DXText::A);
	//AddSymbolTexture(_device, '-', DXText::A);
	//AddSymbolTexture(_device, '.', DXText::A);
	//AddSymbolTexture(_device, '/', DXText::A);
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
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	HRESULT(Device->CreateShaderResourceView(randomTex, &viewDesc, &m_RandomTex));

	ReleaseCOM(randomTex);
}

ID3D11ShaderResourceView* DXTextureManager::buildLetterTex(ID3D11Device *_device, float* _data)
{
	//
	// Create the random data.
	//


	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = _data;
	initData.SysMemPitch = 8 * sizeof(float);
	initData.SysMemSlicePitch = 0;
	//
	// Create the texture.
	//
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 8;
	texDesc.Height = 8;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;


	ID3D11Texture2D* letterTex = 0;
	HRESULT r = HRESULT(_device->CreateTexture2D(&texDesc, &initData, &letterTex));
	//
	// Create the resource view.
	//

	ID3D11ShaderResourceView *letterSRV = NULL;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture2D.MostDetailedMip = 0;
	HRESULT(_device->CreateShaderResourceView(letterTex, &viewDesc, &letterSRV));

	ReleaseCOM(letterTex);
	return letterSRV;
}

void DXTextureManager::AddSymbolTexture(ID3D11Device *_device, char _c, float* _data)
{
	if (mLetterSRV.find(_c) != mLetterSRV.end())
		return;

	mLetterSRV[_c] = buildLetterTex(_device, _data);
}

ID3D11ShaderResourceView* DXTextureManager::GetSymbolTexture(char _c)
{
	ID3D11ShaderResourceView* srv = 0;


	// Does it already exist?
	if (mLetterSRV.find(_c) != mLetterSRV.end())
	{
		srv = mLetterSRV[_c];
	}
	return srv;
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
