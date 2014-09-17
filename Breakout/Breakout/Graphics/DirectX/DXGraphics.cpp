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



	float ClearColor[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor);

	LoadModel("sphere");
	//LoadModel("triangle");

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
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
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
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

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

void DXGraphics::LoadModel(std::string _path)
{
	ModelData* data = FileManager::GetInstance().LoadModel(GetFile(_path, MODEL_ROOT));

	m_testmodel = new DXModel(m_device, m_TextureManager, data);

	m_testmodelinstance = new ModelInstance();

	m_testmodelinstance->SetModel(m_testmodel);

	DirectX::XMMATRIX worldM = DirectX::XMMatrixTranslation(1.5f, 0, 0);
	DirectX::XMFLOAT4X4 world;

	DirectX::XMStoreFloat4x4(&world, worldM);

	float scale = 1.0f;

	m_testmodelinstance->SetWorld(world, world, scale);

	m_DXDeferred->AddModelInstance(m_testmodelinstance);

	m_testmodelinstance = new ModelInstance();

	m_testmodelinstance->SetModel(m_testmodel);

	worldM = DirectX::XMMatrixTranslation(-1.5f, 0, 0);
	DirectX::XMStoreFloat4x4(&world, worldM);

	m_testmodelinstance->SetWorld(world, world, scale);

	m_DXDeferred->AddModelInstance(m_testmodelinstance);

	/*
	if (!data)
	{
		printf("Loadmodel failed: %s\n", _path.c_str());
		return;
	}

	for (std::vector<Group*>::iterator groupIt = data->Groups.begin(); groupIt != data->Groups.end(); ++groupIt)
	{
		int floatSize = (*groupIt)->triangles.size() * 9;

		float* vertexArray = new float[floatSize];
		float* colorArray = new float[floatSize];

		int i = 0;
		for (std::vector<Triangle>::iterator triangleIt = (*groupIt)->triangles.begin(); triangleIt != (*groupIt)->triangles.end(); ++triangleIt)
		{
			//Dest,Source,Size
			memcpy(&vertexArray[3 * i], &(*triangleIt).Vertices[0].Position, sizeof(Vector3));
			memcpy(&colorArray[3 * i], &(*triangleIt).Vertices[0].Normal, sizeof(Vector3));
			memcpy(&vertexArray[3 * (i + 1)], &(*triangleIt).Vertices[1].Position, sizeof(Vector3));
			memcpy(&colorArray[3 * (i + 1)], &(*triangleIt).Vertices[1].Normal, sizeof(Vector3));
			memcpy(&vertexArray[3 * (i + 2)], &(*triangleIt).Vertices[2].Position, sizeof(Vector3));
			memcpy(&colorArray[3 * (i + 2)], &(*triangleIt).Vertices[2].Normal, sizeof(Vector3));

			i += 3;
		}
		printf("Vertices: %d\n", i);
		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ARRAY_BUFFER, floatSize * sizeof(float), vertexArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glGenBuffers(1, &vbo_cube_colors);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
		glBufferData(GL_ARRAY_BUFFER, floatSize * sizeof(float), colorArray, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	std::cout << "Loadmodel finish: " << _path << " with error: " << glGetError() << "\n";
	*/
}


void DXGraphics::Update()
{
	m_window->Update();
}

void DXGraphics::Render(ICamera* _camera)
{
	//float ClearColor[4] = { rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 0.0f };
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->ClearRenderTargetView( m_renderTargetView, ClearColor );


	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	m_DXDeferred->Render(m_renderTargetView, _camera);


	if (FAILED(m_swapChain->Present(0, 0)))
	{
		int a = 2;
	}
}