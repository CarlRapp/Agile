#include "GameScene.h"

GameScene::GameScene()
{
	printf("Game Scene created!\n");
}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	printf("Initialized (Game Scene)\n");

	/*	New Implementation	*/
	m_world = new World();
	m_world->AddSystem<InputSystem>();
	m_world->AddSystem<MovementSystem>();
	m_world->AddSystem<ProjectileSystem>();
	m_world->AddSystem<ModelSystem>();
	m_world->AddSystem<PhysicsSystem>();


	int xBlocks = 16;
	int yBlocks = 3;

	Entity* e;

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(xBlocks + 1 + (xBlocks + 1) * 0.5f, 0, 0));
	m_world->AddEntity(e);

	for (int y = 12; y > 12 - yBlocks; --y)
	for (int x = 1; x < 1 + xBlocks; ++x)
	{
		e = m_world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
		e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(x + x*0.5f, y + y*0.5f, 0));
		m_world->AddEntity(e);
	}

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(12.f, 0, 0));
	m_world->AddEntity(e);


	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(12.f, -10, 0));
	m_world->AddEntity(e);

	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3((xBlocks + 1 + (xBlocks + 1)*0.5f)*0.5f, 8, 35));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));

}

void GameScene::LoadContent()
{
	printf("Loading Content (Game Scene)\n");
}

void GameScene::Update(float _dt)
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
	m_world->Update(_dt);
}

void GameScene::Render()
{
	GraphicsManager::GetInstance()->Render();
}