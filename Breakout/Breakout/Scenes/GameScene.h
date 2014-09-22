#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../ComponentSystem/World.h"
#include "../ComponentSystem/System/ModelSystem.h"
#include "../ComponentSystem/System/PhysicsSystem.h"
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
	Entity* player;
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
		world->AddSystem<PhysicsSystem>();

		int xBlocks = 16;
		int yBlocks = 5;

		Entity* e;
		e = world->CreateEntity();
		e->AddComponent<PositionComponent>(VECTOR3(0, 0, 0));
		e->AddComponent<RotationComponent>();
		e->AddComponent<ScaleComponent>();
		e->AddComponent<ModelComponent>().m_modelPath = "wall";
		e->AddComponent<VelocityComponent>();
		world->AddEntity(e);

		e = world->CreateEntity();
		e->AddComponent<PositionComponent>(VECTOR3(xBlocks + 1+ (xBlocks+1)*0.5f, 0, 0));
		e->AddComponent<RotationComponent>();
		e->AddComponent<ScaleComponent>();
		e->AddComponent<ModelComponent>().m_modelPath = "wall";
		e->AddComponent<VelocityComponent>();
		world->AddEntity(e);

		for (int y = 7; y < 7 + yBlocks; ++y)
			for (int x = 1; x < 1 + xBlocks; ++x)
			{
				e = world->CreateEntity();
				e->AddComponent<PositionComponent>(VECTOR3(x + x*0.5f, y + y*0.5f, 0));
				e->AddComponent<RotationComponent>();
				e->AddComponent<ScaleComponent>();
				e->AddComponent<ModelComponent>().m_modelPath = "box";
				e->AddComponent<VelocityComponent>();
				e->AddComponent<CollisionComponent>();
				world->AddEntity(e);
			}

		player = world->CreateEntity();
		player->AddComponent<PositionComponent>(VECTOR3((xBlocks + 1 + (xBlocks + 1)*0.5f)*0.5f, 1.0f, 0));
		player->AddComponent<RotationComponent>();
		player->AddComponent<ScaleComponent>();
		player->AddComponent<ModelComponent>().m_modelPath = "sphere";
		player->AddComponent<VelocityComponent>();
		world->AddEntity(player);

		GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3((xBlocks + 1 + (xBlocks + 1)*0.5f)*0.5f, 8, 35));
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
		/*
left arrow: 37 
up arrow: 38
right arrow: 39
down arrow: 40
		*/
		float speed = 15.0f;
		float xbounds = 16 + 1 + (16 + 1)*0.5f - 1;
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(37) == InputState::Down)
		{
			VECTOR3 pos = player->GetComponent<PositionComponent>()->m_position;
			if (pos.x -15*_dt < 2)
				player->GetComponent<PositionComponent>()->m_position = VECTOR3(2, pos.y, pos.z);
			else
				player->GetComponent<PositionComponent>()->m_position = VECTOR3(pos.x - 15 * _dt, pos.y, pos.z);
			player->GetComponent<PositionComponent>()->m_deltaPosition = VECTOR3(1, 0, 0);
		}
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(39) == InputState::Down)
		{
			VECTOR3 pos = player->GetComponent<PositionComponent>()->m_position;
			if (pos.x + 15 * _dt > xbounds)
				player->GetComponent<PositionComponent>()->m_position = VECTOR3(xbounds, pos.y, pos.z);
			else
				player->GetComponent<PositionComponent>()->m_position = VECTOR3(pos.x + 15 * _dt, pos.y, pos.z);
			player->GetComponent<PositionComponent>()->m_deltaPosition = VECTOR3(1, 0, 0);
		}

		//if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(32) == InputState::Down)
			world->Update(_dt);
	}

	void Render()
	{
		GraphicsManager::GetInstance()->Render();
	}
};


#endif