#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::m_GMInstance = 0;

GraphicsManager::GraphicsManager(void)
{
	m_IGraphics = IGraphics::GetIGraphics();
}

GraphicsManager* GraphicsManager::GetInstance()
{
	if (m_GMInstance)
		return m_GMInstance;

	m_GMInstance = new GraphicsManager();

	return m_GMInstance;
}

void GraphicsManager::Destroy()
{
	SafeDelete(m_GMInstance);
}

GraphicsManager::~GraphicsManager(void)
{
    SafeDelete(m_IGraphics);
}

bool GraphicsManager::InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode)
{

	m_ICamera = ICamera::GetICamera(45.0f,_width,_height,0.1f,100.0f);
	m_ICamera->SetPosition(VECTOR3(0, 0, 10));

	m_ICamera->SetLookAt(VECTOR3(0, 0, 0));

	m_ICamera->SetViewPort(0, 0, _width, _height);
	return m_IGraphics->InitWindow(_x, _y, _width, _height, _displayMode);
}


void GraphicsManager::Render()
{
	m_IGraphics->Render(m_ICamera);
}

float test = 0.0f;
void GraphicsManager::Update()
{
	m_IGraphics->Update();

	VECTOR3 pos = m_ICamera->GetPosition();
	//pos.z += dt * 10.0f;

	test += 0.001f * 0.4;

	//pos.x = 5 * sinf(test);
	//pos.z = 5 * cosf(test);

	//m_ICamera->SetPosition(pos);
	//m_ICamera->SetLookAt(VECTOR3(0, 0, 0));
}


bool GraphicsManager::Init3D(DisplayMode _displayMode)
{
    return m_IGraphics->Init3D(_displayMode);
}

void GraphicsManager::AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range)
{
    m_IGraphics->AddPointLight(_id, _worldPos, _intensity, _color, _range);
}
void GraphicsManager::RemovePointLight(int _id)
{
	m_IGraphics->RemovePointLight(_id);
}
   

void GraphicsManager::AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose, float* _explosion)
{
	m_IGraphics->AddObject(_id, _model, _world, _worldInverseTranspose,_explosion);
}

void GraphicsManager::RemoveObject(int _id)
{
	m_IGraphics->RemoveObject(_id);
}


void GraphicsManager::Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height)
{
	m_IGraphics->Add2DTexture(_id, _path, _x, _y, _width, _height);
}

void GraphicsManager::Remove2DTexture(int _id)
{
	m_IGraphics->Remove2DTexture(_id);
}


ICamera* GraphicsManager::GetICamera()
{
    return m_ICamera;
}

void GraphicsManager::AddTextObject(std::string* _text,float* _scale, unsigned int* _color,int* _x,int* _y)
{
    m_IGraphics->AddTextObject(_text,_scale,_color,_x,_y);
}