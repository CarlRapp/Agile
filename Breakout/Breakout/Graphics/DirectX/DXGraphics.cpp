#include "DXGraphics.h"
#include "../../Storage/FileManager.h"
#include "DXModel.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


DXGraphics::DXGraphics(void)
{
}

DXGraphics::~DXGraphics(void)
{
	m_modelInstances.clear();

	DXRenderStates::DestroyAll();
	DXEffects::DestroyAll();
	DXInputLayouts::DestroyAll();

	SafeDelete(m_DXDeferred);
	SafeDelete(m_window);
	ReleaseCOM(m_swapChain);
	ReleaseCOM(m_renderTargetView);
	ReleaseCOM(m_depthStencil);
	ReleaseCOM(m_depthStencilView);
	ReleaseCOM(m_device);
	ReleaseCOM(m_deviceContext);
	ReleaseCOM(m_finalUAV);
}

bool DXGraphics::InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode)
{
	m_width = _width;
	m_height = _height;

	m_window = new DXWindow();
	if (!m_window->InitWindow(_x, _y, _width, _height, _displayMode))
		return false;
	return true;
	
}




//#define asd 1000
//MATRIX4 world[asd];
bool DXGraphics::Init3D(DisplayMode _displayMode)
{
	if (FAILED(InitDirect3D(_displayMode)))
	{
		::MessageBox(0, "Failed to initalize Direct3D", "Error", MB_OK);
		return false;
	}

	DXRenderStates::InitAll(m_device);
	DXEffects::InitAll(m_device);
	DXInputLayouts::InitAll(m_device);

	m_DXDeferred = new DXDeferred();
	m_DXDeferred->Init(m_device, m_deviceContext, m_width, m_height);

	m_DXDeferred->SetPointLightMap(&m_pointLights);

	

	m_textureManager.Init(m_device);

	float ClearColor[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor);



	//for (int i = 0; i < asd; ++i)
	//{
	//	DirectX::XMStoreFloat4x4(&world[i], DirectX::XMMatrixTranslation(-50 + (i % 50) * 2.0f, 20-((i / 50) * 2), 0));

	//	AddObject(i, "sphere", &world[i], &world[i]);
	//}


	return true;
}

HRESULT DXGraphics::InitDirect3D(DisplayMode _displayMode)
{
	HRESULT hr = S_OK;;

	int screenWidth = m_window->GetWidth();
	int screenHeight = m_window->GetHeight();

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]); //same as ARRAYSIZE(x) macro

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	sd.OutputWindow = m_window->GetHandle();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	if (_displayMode == DisplayMode::Fullscreen)
		sd.Windowed = false;
	else
		sd.Windowed = true;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&sd,
			&m_swapChain,
			&m_device,
			&initiatedFeatureLevel,
			&m_deviceContext);

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	ID3D11Texture2D* pBackBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
	
	if (FAILED(hr))
		return hr;


	//Create the UnorderedAccessView
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	hr = m_device->CreateUnorderedAccessView(pBackBuffer, 0, &m_finalUAV);
	if (FAILED(hr))
		return hr;

	pBackBuffer->Release();


	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = screenWidth;
	descDepth.Height = screenHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_device->CreateTexture2D(&descDepth, NULL, &m_depthStencil);
	if (FAILED(hr))
		return hr;


	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_device->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
	if (FAILED(hr))
		return hr;


	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)screenWidth;
	vp.Height = (float)screenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &vp);

	return S_OK;
}

float asdasdddd = 1.5f;
void DXGraphics::LoadModel(std::string _path)
{
	m_modelManager.LoadModel(m_device, _path, m_textureManager);
}

void DXGraphics::LoadTexture(std::string _path)
{
	m_textureManager.CreateTexture(_path);
}


void DXGraphics::Update()
{
	m_window->Update();
}

void DXGraphics::Render(ICamera* _camera)
{
	//float ClearColor[4] = { rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 0.0f };
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor);


	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	m_DXDeferred->Render(m_renderTargetView, m_finalUAV, m_modelInstances, m_textureInstances, _camera);

	m_swapChain->Present(0, 0);

}


void DXGraphics::AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose, float *_explosionTime)
{

	if (m_modelInstances[_model].count(_id) != 0)
		return;

	LoadModel(_model);

	ModelInstance *mi = new ModelInstance();

	mi->model = m_modelManager.GetModel(_model);
	mi->world = _world;
	mi->worldInverseTranspose = _worldInverseTranspose;
	mi->explodeTime = _explosionTime;

	m_modelInstances[_model].insert(pair<int, ModelInstance*>(_id, mi));
}


void DXGraphics::RemoveObject(int _id)
{

	map<std::string, map<int, ModelInstance*>>::iterator	mapIterator;
	for (mapIterator = m_modelInstances.begin(); mapIterator != m_modelInstances.end(); ++mapIterator)
	{
		mapIterator->second.erase(_id);
	}
}

void DXGraphics::Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height)
{
	if (m_textureInstances.count(_id) != 0)
		return;

	DX2DTextureInstance *ti = new DX2DTextureInstance();

	ti->Texture = m_textureManager.CreateTexture(_path);
	ti->X = _x;
	ti->Y = _y;
	ti->Width = _width;
	ti->Height = _height;

	m_textureInstances.insert(pair<int, DX2DTextureInstance*>(_id, ti));
}

void DXGraphics::Remove2DTexture(int _id)
{
	m_textureInstances.erase(_id);
}


void DXGraphics::AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range)
{
	if (m_pointLights.count(_id) != 0)
		return;

	PointLight *pl = new PointLight();

	pl->Color = _color;
	pl->Position = _worldPos;
	pl->Range = _range;
	pl->Intensity = _intensity;

	m_pointLights.insert(pair<int, PointLight*>(_id, pl));
}

void DXGraphics::RemovePointLight(int _id)
{
	m_pointLights.erase(_id);
}