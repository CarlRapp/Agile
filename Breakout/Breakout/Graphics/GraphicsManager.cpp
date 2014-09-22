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

GraphicsManager::~GraphicsManager(void)
{
    SafeDelete(m_IGraphics);
}

bool GraphicsManager::InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode)
{
	m_ICamera = ICamera::GetICamera(45.0f,_width,_height,0.1f,1000.0f);

	m_ICamera->SetPosition(VECTOR3(0, 0, 10));

	m_ICamera->SetLookAt(VECTOR3(0, 0, 0));

	m_ICamera->SetViewPort(0, 0, _width, _height);
	return m_IGraphics->InitWindow(_x, _y, _width, _height, _displayMode);

}


void GraphicsManager::Render()
{
	return m_IGraphics->Render(m_ICamera);
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
	m_ICamera->SetForward(VECTOR3(0, 0, -1));
}


bool GraphicsManager::Init3D(DisplayMode _displayMode)
{
    return m_IGraphics->Init3D(_displayMode);
}

void GraphicsManager::AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose)
{
	m_IGraphics->AddObject(_id, _model, _world, _worldInverseTranspose);
}

void GraphicsManager::RemoveObject(int _id)
{
	m_IGraphics->RemoveObject(_id);
}

ICamera* GraphicsManager::GetICamera()
{
    return m_ICamera;
}

void GraphicsManager::AddRenderObject(std::string _path, MATRIX4 _world)
{

}