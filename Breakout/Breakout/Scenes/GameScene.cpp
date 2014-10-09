#include "GameScene.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"

#include "../ComponentSystem/System/BlockSystem.h"
#include "../ComponentSystem/Component/RotationComponent.h"
#include "../ComponentSystem/System/LoseLifeSystem.h"
#include "../ComponentSystem/Component/TextComponent.h"

float counter;
std::string fpsString= "FPS: ";

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
	
	//AudioManager::GetInstance()->PlayMusic("Tetris.wav", -1);

	m_pauseBackground = new Texture2DData();
	m_pauseBackground->m_positionX = 0;
	m_pauseBackground->m_positionY = 0;
	m_pauseBackground->m_imageWidth = 1;
	m_pauseBackground->m_imageHeight = 1;
	m_pauseBackground->m_textureName = "Pause.png";

	counter = 0;
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
	if (counter > 1.f)
	{
		Entity* e;
		e = m_world->CreateEntity();

		int rnd = (rand() % (100 - 0));

		EntityFactory::EntityType type;

		if (rnd >= 0 && rnd < 5)
			type = EntityFactory::STANDARD_BLOCK_RED;
		else if(rnd >= 5 && rnd < 10)
			type = EntityFactory::STANDARD_BLOCK_GREEN;
		else if(rnd >= 10 && rnd < 15)
			type = EntityFactory::STANDARD_BLOCK_BLUE;
		else if (rnd >= 15 && rnd < 20)
			type = EntityFactory::STANDARD_HORIZONTAL_RECTANGLE;
		else if (rnd == 20)
			type = EntityFactory::INDESTRUCTIBLE_BLOCK;
		else
			type = EntityFactory::TNT_BLOCK;

		EntityFactory::GetInstance()->CreateEntity(e, type);
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
        
        float fps = 1.0f / _dt;
        fpsString= "FPS: ";
        fpsString.append(std::to_string(fps));
        fpsString += " DT: ";
        fpsString.append(std::to_string(_dt));
}

void GameScene::Render(float _dt)
{
	if (m_isPaused)
		GraphicsManager::GetInstance()->Render(0);
	else
		GraphicsManager::GetInstance()->Render(_dt);
}

void GameScene::OnActive()
{
	Reset();
}
void GameScene::OnInactive()
{
	GraphicsManager::GetInstance()->Remove2DTexture(GetMemoryID(m_pauseBackground));
	m_isPaused = false;
	if (m_world)
	{
		EntityMap::iterator eIT;
		for (eIT = m_world->GetAllEntities()->begin(); eIT != m_world->GetAllEntities()->end(); ++eIT)
		{
			eIT->second->SetInitialized(false);
			GraphicsManager::GetInstance()->RemoveObject(GetMemoryID(eIT->second));
			GraphicsManager::GetInstance()->RemovePointLight(GetMemoryID(eIT->second));
			GraphicsManager::GetInstance()->RemoveParticleEffect(GetMemoryID(eIT->second));
			GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(eIT->second));
                        
                        if(eIT->second->HasComponent<TextComponent>())
                        {
                            auto TC = eIT->second->GetComponent<TextComponent>();
                            TC->m_text = nullptr;
                        }
		}	

		delete m_world;
	}
}

void GameScene::Reset()
{
	/*	New Implementation	*/
	m_world = new World();
	m_world->AddSystem<InputSystem>();
	m_world->AddSystem<PhysicsSystem>();
	m_world->AddSystem<ModelSystem>();
	m_world->AddSystem<MovementSystem>();
	//m_world->AddSystem<ProjectileSystem>();
	m_world->AddSystem<ScoreSystem>();
	m_world->AddSystem<AudioSystem>();
	m_world->AddSystem<CollisionDeflectionSystem>();
	m_world->AddSystem<CollisionDamageSystem>();
	m_world->AddSystem<LoseLifeSystem>();
	m_world->AddSystem<RespawnBallSystem>();
	m_world->AddSystem<LightSystem>();
	m_world->AddSystem<EffectSystem>();
	m_world->AddSystem<BlockSystem>();

	/*	New Implementation	*/
        
    //FPS COUNTER
    Entity* e;
        
    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    auto TC = e->GetComponent<TextComponent>();
    TC->Initialize(&fpsString, 0.1f, 0.9f, 2.f, VECTOR3(0,1,0), 0);
    m_fpsCounterID = e->GetId();
    m_world->AddEntity(e);
	GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
        
	for (int i = 0; i < 50; ++i)
	{
		e = m_world->CreateEntity();
		int rnd = (rand() % (3 - 0));
		EntityFactory::GetInstance()->CreateEntity(e, (EntityFactory::EntityType)rnd);
		e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
		m_world->AddEntity(e);
	}
	/*e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -20, 0));
	m_world->AddEntity(e);*/

	Entity* midPad = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(midPad, EntityFactory::SAUSAGE_PAD_MID);
	midPad->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -20, 0));
	midPad->GetComponent<ScaleComponent>()->SetScale(VECTOR3(10, 2, 2));
	m_world->AddEntity(midPad);
	
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::SAUSAGE_PAD_EDGE);
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(midPad->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, -20, 0));
	e->GetComponent<RotationComponent>()->SetRotation(ROTATEYAWPITCHROLLFROMVECTOR(VECTOR3(0, PI * 0.5f, 0)));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::SAUSAGE_PAD_EDGE);
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-midPad->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, -20, 0));
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
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-47 - e->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(47 + e->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLAYER);
	m_world->AddEntity(e);

        //PLAYER SCORE >>
    auto SC = e->GetComponent<ScoreComponent>();
        
    Entity* t = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(t, EntityFactory::TEXT);
    TC = t->GetComponent<TextComponent>();
        
    TC->Initialize(SC->GetString(),0.1f, 0.8f, 2.f, VECTOR3(0,1,1), 0);
    m_world->AddEntity(e);
	GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
	SC->SetString();
        //PLAYER SCORE <<
        
	GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
//        //PLAYER SCORE <<
        
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