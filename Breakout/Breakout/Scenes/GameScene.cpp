#include "GameScene.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"

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
	m_world->AddSystem<BounceSystem>();
	m_world->AddSystem<ScoreSystem>();
	m_world->AddSystem<AudioSystem>();
	m_world->AddSystem<CollisionDamageSystem>();
	m_world->AddSystem<PlayerLifeSystem>();
	m_world->AddSystem<LightSystem>();

	m_pauseBackground = new Texture2DData();
	m_pauseBackground->m_positionX = 0;
	m_pauseBackground->m_positionY = 0;
	m_pauseBackground->m_imageWidth = 1;
	m_pauseBackground->m_imageHeight = 1;
	m_pauseBackground->m_textureName = "Pause.png";

	m_isPaused = false;
}

void GameScene::LoadContent()
{
	printf("Loading Content (Game Scene)\n");

	//GraphicsManager::GetInstance()->GetIGraphics()->LoadModel("sphere");
}

void GameScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
	{
		m_isPaused = !m_isPaused;
		GraphicsManager* GM = GraphicsManager::GetInstance();

		if (m_isPaused)
			GM->Add2DTexture(
				GetMemoryID(m_pauseBackground), m_pauseBackground->m_textureName, 
				&m_pauseBackground->m_positionX, &m_pauseBackground->m_positionY, 
				&m_pauseBackground->m_imageWidth, &m_pauseBackground->m_imageHeight
			);
		else
			GM->Remove2DTexture(GetMemoryID(m_pauseBackground));
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

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('c') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(-10 * _dt);
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
		SceneManager::GetInstance()->ChangeScene<GameOverScene>();
	}
}

void GameScene::Render()
{
	GraphicsManager::GetInstance()->Render();
}

void GameScene::OnActive()
{
	m_world->SetAlive();

	Reset();
}
void GameScene::OnInactive()
{
	EntityMap::iterator eIT;
	for (eIT = m_world->GetAllEntities()->begin(); eIT != m_world->GetAllEntities()->end(); ++eIT)
	{
		eIT->second->SetInitialized(false);
		GraphicsManager::GetInstance()->RemoveObject(GetMemoryID(eIT->second));
	}	
	
	GraphicsManager::GetInstance()->Remove2DTexture(GetMemoryID(m_pauseBackground));
	m_isPaused = false;
}

void GameScene::Reset()
{
	m_world->Clear();

	/*	New Implementation	*/
	Entity* e;
	int xBlock, yBlock;
	int	startX, startY;

	startX = -20;
	xBlock = 2 * -startX;
	startY = 6;
	yBlock = 4;

	for (int y = startY; y < startY + yBlock; ++y)
		for (int x = startX; x < startX + xBlock; ++x)
		{
			e = m_world->CreateEntity();
			EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
			e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(x + x*0.5f, y + y*0.5f, 0));
			e->GetComponent<RotationComponent>()->SetRotation(MacroRotateYawPitchRoll(0 ,0, 0));
			m_world->AddEntity(e);
		}

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -20, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 0, 20));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::INVISIBLE_WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -25, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::H_WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 28, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-47, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(47, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLAYER);
	m_world->AddEntity(e);


	//	Background
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLANE);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-50, -30, -5));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(100, 60, 1));
	m_world->AddEntity(e);

	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3(0, 0, 70));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(500, 300);

}