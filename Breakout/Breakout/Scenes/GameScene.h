#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../ComponentSystem/World.h"
#include "../ComponentSystem/System/ModelSystem.h"
#include "../ComponentSystem/System/MovementSystem.h"
#include "../ComponentSystem/System/InputSystem.h"
#include "../ComponentSystem/System/CollisionSystem.h"
#include "../ComponentSystem/System/ProjectileSystem.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../ComponentSystem/EntityFactory.h"
using namespace SceneSystem;

class GameScene : public Scene<GameScene>
{
private:
	World* world;
	//Entity* player;
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
		world->AddSystem<InputSystem>();
		world->AddSystem<MovementSystem>();
		world->AddSystem<ProjectileSystem>();
		//world->AddSystem<CollisionSystem>();
		world->AddSystem<ModelSystem>();

		int xBlocks = 16;
		int yBlocks = 3;

		Entity* e;

		e = world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
		world->AddEntity(e);

		e = world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
		e->GetComponent<PositionComponent>()->m_position = VECTOR3(xBlocks + 1 + (xBlocks + 1) * 0.5f, 0, 0);
		world->AddEntity(e);

		for (int y = 12; y > 12 - yBlocks; --y)
			for (int x = 1; x < 1 + xBlocks; ++x)
			{
				e = world->CreateEntity();
				EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
				e->GetComponent<PositionComponent>()->m_position = VECTOR3(x + x*0.5f, y + y*0.5f, 0);
				e->GetComponent<CollisionComponent>()->m_position = VECTOR2(0, 0);
				e->GetComponent<CollisionComponent>()->m_size = VECTOR2(1, 1);
				world->AddEntity(e);
			}

		e = world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BALL);
		e->GetComponent<CollisionComponent>()->m_position = VECTOR2(0, 0);
		e->GetComponent<CollisionComponent>()->m_size = VECTOR2(2, 2);
		e->GetComponent<CollisionComponent>()->m_offset = VECTOR2(-1, -1);
		world->AddEntity(e);




		e = world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
		e->GetComponent<PositionComponent>()->m_position = VECTOR3(8, 0, 0);
		e->GetComponent<CollisionComponent>()->m_position = VECTOR2(0, 0);
		e->GetComponent<CollisionComponent>()->m_size = VECTOR2(1, 1);
		world->AddEntity(e);

		GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3((xBlocks + 1 + (xBlocks + 1)*0.5f)*0.5f, 8, 35));
		GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));

		ModelData* lol = FileManager::GetInstance().LoadModel(GetFile("box", MODEL_ROOT));

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
	/*	float speed = 15.0f;
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

		if (InputManager::GetInstance()->getInputDevices()->GetMouse()->GetButtonState(MouseButton::LeftMB) == InputState::Down)
			printf("LOL");
		if (InputManager::GetInstance()->getInputDevices()->GetMouse()->GetButtonState(MouseButton::RightMB) == InputState::Down)
			printf("LOL");
		if (InputManager::GetInstance()->getInputDevices()->GetMouse()->GetButtonState(MouseButton::MiddleMB) == InputState::Down)
			printf("LOL");

		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(32) == InputState::Pressed)
		{
			player->GetComponent<VelocityComponent>()->m_velocity.y *= -1;

		}*/
			world->Update(_dt);
	}

	void Render()
	{
		GraphicsManager::GetInstance()->Render();
	}
};


#endif