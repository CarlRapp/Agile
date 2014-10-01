#include "GameScene.h"
#include "MainMenuScene.h"

#include "../ComponentSystem/Component/RotationComponent.h"
#include "../ComponentSystem/System/PlayerLifeSystem.h"

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
	m_world->AddSystem<PhysicsSystem>();
	m_world->AddSystem<ModelSystem>();
	m_world->AddSystem<MovementSystem>();
	m_world->AddSystem<ProjectileSystem>();
	m_world->AddSystem<CollisionDeflectionSystem>();
	m_world->AddSystem<ScoreSystem>();
	m_world->AddSystem<AudioSystem>();
	m_world->AddSystem<CollisionDamageSystem>();
	m_world->AddSystem<PlayerLifeSystem>();
	
	m_world->AddSystem<LightSystem>();


	int xBlocks = 30;
	int yBlocks = 3;

	Entity* e;

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLAYER);
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(xBlocks + 1 + (xBlocks + 1) * 0.5f, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::H_WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3((16.5 + xBlocks) * 0.5f, 15, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::INVISIBLE_WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3((16.5f + xBlocks) * 0.5f, -25, 0));
	m_world->AddEntity(e);

	for (int y = 9; y > 9 - yBlocks; --y)
	for (int x = 1; x < 1 + xBlocks; ++x)
	{
		e = m_world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
		e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(x + x*0.5f, y + y*0.5f, 0));
		m_world->AddEntity(e);
	}


	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3((16 + xBlocks) * 0.5f, -10, 0)); 
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3((28 + 28 * 0.5f)*0.5f, 5, 20));
	m_world->AddEntity(e);

	/*	PLANE	*/
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLANE);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0,-15,5));
	//e->GetComponent<RotationComponent>()->SetRotation(VECTOR3(0, 0, 15));
	m_world->AddEntity(e);


	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3((xBlocks + 1 + (xBlocks + 1)*0.5f)*0.5f, 1.10f, 35));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(500, 300);

	m_x = 0;
	m_y = 0;
	m_width = 1.f;
	m_height = 1.f;
	m_isPaused = false;
}

void GameScene::LoadContent()
{
	printf("Loading Content (Game Scene)\n");

	GraphicsManager::GetInstance()->GetIGraphics()->LoadModel("sphere");
}

void GameScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
	{
		m_isPaused = !m_isPaused;
		GraphicsManager* GM = GraphicsManager::GetInstance();

		if (m_isPaused)
			GM->Add2DTexture(1338, "Pause.png", &m_x, &m_y, &m_width, &m_height);
		else
			GM->Remove2DTexture(1338);
		return;
	}
	if (m_isPaused)
	{
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
			SceneManager::GetInstance()->ChangeScene<MainMenuScene>();

		return;
	}
		

	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(500, 500);

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('r') == InputState::Pressed)
		this->Reset();

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('a') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(VECTOR3(-50 * _dt, 0, 0));
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('d') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(VECTOR3(50 * _dt, 0, 0));
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('w') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(-50 * _dt);
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('s') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(50 * _dt);

	m_world->Update(_dt);

	if (!m_world->IsAlive())
	{
		SceneManager::GetInstance()->ChangeScene<MainMenuScene>();
	}
}

void GameScene::Render()
{
	GraphicsManager::GetInstance()->Render();
}

void GameScene::OnActive()
{
	m_world->SetAlive();
}
void GameScene::OnInactive()
{
	EntityMap::iterator eIT;
	for (eIT = m_world->GetAllEntities()->begin(); eIT != m_world->GetAllEntities()->end(); ++eIT)
	{
		eIT->second->SetInitialized(false);
	}
	for (int i = 0; i < 500; ++i)
		GraphicsManager::GetInstance()->RemoveObject(i);
	
	GraphicsManager::GetInstance()->Remove2DTexture(1338);
	m_isPaused = false;
}

void GameScene::Reset()
{

}