#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../ComponentSystem/World.h"
#include "../ComponentSystem/System/ModelSystem.h"
#include "../ComponentSystem/Component/PositionComponent.h"
#include "../ComponentSystem/Component/RotationComponent.h"
#include "../ComponentSystem/Component/ScaleComponent.h"
#include "../ComponentSystem/Component/ModelComponent.h"
#include "../ComponentSystem/Component/VelocityComponent.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
using namespace SceneSystem;

class GameScene : public Scene<GameScene>
{
private:
	World* world;
public:
	GameScene()
	{
		printf("Game Scene created!\n");
	}

	void Initialize()
	{
		printf("Initialized (Game Scene)\n");

		/*	New Implementation	*/
		world = new World();
		world->AddSystem<ModelSystem>();

		Entity* e = world->CreateEntity();
		world->AddEntity(e);
		e = world->CreateEntity();
		world->AddEntity(e);
		e = world->CreateEntity();
		world->AddEntity(e);

		for (int y = 5; y < 10; ++y)
			for (int x = 0; x < 10; ++x)
			{
				e = world->CreateEntity();
				e->AddComponent<PositionComponent>(VECTOR3(x + x*0.5f, y + y*0.5f, 0));
				e->AddComponent<RotationComponent>();
				e->AddComponent<ScaleComponent>();
				e->AddComponent<ModelComponent>().m_modelPath = "box";
				e->AddComponent<VelocityComponent>();
				world->AddEntity(e);
			}

		GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));

		ModelData* lol = FileManager::GetInstance().LoadModel(GetFile("box", MODEL_ROOT));
		int a = 2;
	}

	void LoadContent()
	{
		printf("Loading Content (Game Scene)\n");
	}

	void Update(float _dt)
	{
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
			SceneManager::GetInstance()->Quit();

		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('a') == InputState::Down)
			GraphicsManager::GetInstance()->GetICamera()->Move(VECTOR3(-50 * _dt, 0, 0));
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('d') == InputState::Down)
			GraphicsManager::GetInstance()->GetICamera()->Move(VECTOR3(50 * _dt, 0, 0));
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('w') == InputState::Down)
			GraphicsManager::GetInstance()->GetICamera()->Move(50 * _dt);
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('s') == InputState::Down)
			GraphicsManager::GetInstance()->GetICamera()->Move(-50 * _dt);


		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(32) == InputState::Down)
			world->Update(_dt);
	}

	void Render()
	{
		GraphicsManager::GetInstance()->Render();
	}
};


#endif