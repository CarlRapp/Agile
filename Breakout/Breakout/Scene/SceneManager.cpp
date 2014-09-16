#include "SceneManager.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
using namespace SceneSystem;

SceneManager::SceneManager()
{
	m_scenes = SceneMap();
	m_activeScene = 0;
	m_isRunning = true;
}
SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager* mInstance = new SceneManager();
	return mInstance;
}

void SceneManager::InitScene(BaseScene* _scene)
{
	if (_scene == NULL)
		return;

	//	Initialize and load scene
	_scene->Initialize();
	_scene->LoadContent();
}

float SceneManager::GetDeltaTime()
{

#ifdef WINDOWS
	QueryPerformanceCounter(&m_t2);
	double dt = (m_t2.QuadPart - m_t1.QuadPart) * 1000.0 / m_frequency.QuadPart;
	QueryPerformanceCounter(&m_t1);
	return (float)(dt*0.001f);
#else
	/* IMPLEMENT SHIT HERE */
	gettimeofday(&m_t2);
	double dt = (t2.tv_sec - t1.tv_sec)*1000.0f;
	dt += (t2.tv_usec - t1.tv_usec) * 0.001f;
	gettimeofday(&m_t1);
	return dt;
#endif
}

void SceneManager::Start()
{
#ifdef WINDOWS
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_t1);
#else
	// Start linux timer here
	gettimeofday(&m_t1);
#endif

	float lol = 0;
	InputManager* IM = InputManager::GetInstance();
	GraphicsManager* GM = GraphicsManager::GetInstance();

	while (m_isRunning)
	{
		float dt = GetDeltaTime();
		GM->Update();
		IM->Update();

		m_activeScene->Update(dt);
		m_activeScene->Render();

		lol += dt;
		if (lol > 1)
		{
			printf("FPS: %f\n", 1.0f / dt);
			printf("Delta: %f\n", dt);
			lol = 0;
		}
	}

}