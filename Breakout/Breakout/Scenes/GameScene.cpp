#include "GameScene.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"

#include "../ComponentSystem/System/BlockSystem.h"
#include "../ComponentSystem/Component/RotationComponent.h"
#include "../ComponentSystem/System/LoseLifeSystem.h"

float counter;
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
	m_world->AddSystem<ScoreSystem>();
	m_world->AddSystem<AudioSystem>();
	m_world->AddSystem<EffectSystem>();
	m_world->AddSystem<CollisionDamageSystem>();
	m_world->AddSystem<CollisionDeflectionSystem>();
	//m_world->AddSystem<LoseLifeSystem>();
	//m_world->AddSystem<RespawnBallSystem>();
	m_world->AddSystem<LightSystem>();
	m_world->AddSystem<BlockSystem>();
	

	m_pauseBackground = new Texture2DData();
	m_pauseBackground->m_positionX = 0;
	m_pauseBackground->m_positionY = 0;
	m_pauseBackground->m_imageWidth = 1;
	m_pauseBackground->m_imageHeight = 1;
	m_pauseBackground->m_textureName = "Pause.png";

	counter = 0;

//        //FPS COUNTER
//        e = m_world->CreateEntity();
//        EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
//        auto TC = e->GetComponent<TextComponent>();
//        TC->Initialize("FPS: ",2.f,0x1904 ,10,10);
//        m_fpsCounterID = e->GetId();
//        m_world->AddEntity(e);
//        GraphicsManager::GetInstance()->AddTextObject(&TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y);
//        
//	m_x = 0;
//	m_y = 0;
//	m_width = 1.f;
//	m_height = 1.f;

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
		GraphicsManager::GetInstance()->GetICamera()->Move(10 * _dt);
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('a') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(VECTOR3(-50 * _dt, 0, 0));
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('d') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(VECTOR3(50 * _dt, 0, 0));
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('w') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(50 * _dt);
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('s') == InputState::Down)
		GraphicsManager::GetInstance()->GetICamera()->Move(-50 * _dt);

	counter += _dt;
	if (counter > .5f)
	{
		Entity* e;
		e = m_world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
		e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
		m_world->AddEntity(e);
		counter = 0;
	}

	m_world->Update(_dt);

	if (!m_world->IsAlive())
	{
		SceneManager::GetInstance()->ChangeScene<GameOverScene>();
	}

        UpdateFPS(_dt);
        
}

void GameScene::UpdateFPS(float _dt)
{
  //      
  //      float fps = 1.0f / _dt;
  //      std::string fpsString= "FPS: ";
  //      fpsString.append(std::to_string(fps));
  //      fpsString += " DT: ";
  //      fpsString.append(std::to_string(_dt));
  //      Entity* e = m_world->GetEntity(m_fpsCounterID);
  //      auto TC = e->GetComponent<TextComponent>();
		//if(TC)
		//	TC->SetText(fpsString);
}

void GameScene::Render(float _dt)
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
	for (int i = 0; i < 10; ++i)
	{
		e = m_world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
		e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
		m_world->AddEntity(e);
	}
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -20, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 0, 20));
	e->GetComponent<LightComponent>()->SetColor(VECTOR3(0.7f, 0.7f, 0.7f));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 0, -20));
	e->GetComponent<LightComponent>()->SetColor(VECTOR3(0.7f, 0.7f, 0.7f));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-50, 0, 0));
	e->GetComponent<LightComponent>()->SetColor(VECTOR3(0.7f, 0.7f, 0.7f));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(50, 0, 0));
	e->GetComponent<LightComponent>()->SetColor(VECTOR3(0.7f, 0.7f, 0.7f));
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

	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3(0, 1, 67));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(500, 300);

}