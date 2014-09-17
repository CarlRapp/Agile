#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "Scene.h"
#include <time.h>
#include <vector>

#ifdef WINDOWS
#include <Windows.h>
#else
#include <SDL/SDL.h>
#include <sys/time.h>
#endif

namespace SceneSystem
{

	class SceneManager
	{
	public:
		static SceneManager* GetInstance();
		~SceneManager();

		template <typename T, typename... Args>
		bool AddScene(bool _change, Args&&... _args);

		template <typename T>
		bool ChangeScene();

		void Start();

		void Quit()
		{
			m_isRunning = false;
		}
	private:
		SceneManager();
		BaseScene*	m_activeScene;



		SceneMap m_scenes;
		bool m_isRunning;

		template <typename T>
		bool HasScene() const;

		void InitScene(BaseScene* _scene);
		float GetDeltaTime();

		/* OS DEPENDENT SHIT */
#ifdef WINDOWS
		LARGE_INTEGER m_frequency;
		LARGE_INTEGER m_t1, m_t2;
#else
		// Fix on unix
#define TimeVal struct timeval
#define TimeZone struct	timezone
                TimeVal	m_t1, m_t2;
                TimeZone m_tZone;
#endif

		
	};

	template <typename T>
	bool SceneManager::HasScene() const
	{
		return m_scenes.count(T::GetSceneID()) != 0;
	}

	template <typename T, typename... Args>
	bool SceneManager::AddScene(bool _change, Args&&... _args)
	{
		if (HasScene<T>())
			return 0;

		m_scenes[T::GetSceneID()] = new T{ std::forward<Args>(_args)... };
		InitScene(m_scenes[T::GetSceneID()]);

		if (_change)
			return ChangeScene<T>();

		return true;
	}

	template <typename T>
	bool SceneManager::ChangeScene()
	{
		m_activeScene = m_scenes[T::GetSceneID()];

		return true;
	}

}

#endif