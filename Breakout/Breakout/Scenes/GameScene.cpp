#include "GameScene.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"

#include "../ComponentSystem/System/BlockSystem.h"
#include "../ComponentSystem/Component/RotationComponent.h"
#include "../ComponentSystem/System/LoseLifeSystem.h"
#include "../ComponentSystem/Component/TextComponent.h"
#include "../ComponentSystem/System/TextSystem.h"
#include "../ComponentSystem/System/PadCollisionSystem.h"
#include "../ComponentSystem/System/SpawnPowerUpSystem.h"
#include "../ComponentSystem/System/CollectPowerUpSystem.h"
#include "../ComponentSystem/System/KillOnTouchSystem.h"
#include "../ComponentSystem/Component/BallComponent.h"

float counter;
std::string m_fpsString= "FPS: ";
std::string m_pauseString= "GAME PAUSED";
std::string m_gameOverString= "GAME OVER";

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
}

void GameScene::Update(float _dt)
{
    UpdateFPS(_dt);
    
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
	{
		m_isPaused = !m_isPaused;
		GraphicsManager* GM = GraphicsManager::GetInstance();

		if (m_isPaused)
                {
                   
                    if(!m_gameOver)
                    {
                        Entity* e = m_world->GetEntity(m_pauseHandle);
                        auto TC = e->GetComponent<TextComponent>();

                        GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
                    }
                    
		}
                else
                {
                    Entity* e = m_world->GetEntity(m_pauseHandle);
                    GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(e));
                    //GM->Remove2DTexture(GetMemoryID(m_pauseBackground));
                }
		return;
	}
	if (m_isPaused)
	{
		if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
			SceneManager::GetInstance()->ChangeScene<MainMenuScene>();

                m_world->UpdateTextOnly(_dt);
                
		return;
	}
        
        
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(0.5f, 0.5f);

        
	//if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('r') == InputState::Pressed)
	//	this->Reset();

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

		if (rnd >= 0 && rnd < 20)
			type = EntityFactory::STANDARD_BLOCK_RED;
		else if (rnd >= 20 && rnd < 40)
			type = EntityFactory::STANDARD_BLOCK_GREEN;
		else if (rnd >= 40 && rnd < 60)
			type = EntityFactory::STANDARD_BLOCK_BLUE;
		else if (rnd >= 60 && rnd < 80)
			type = EntityFactory::STANDARD_HORIZONTAL_RECTANGLE;
		else if (rnd >= 80 && rnd < 90)
			type = EntityFactory::INDESTRUCTIBLE_BLOCK;
		else if (rnd >= 90 && rnd < 100)
			type = EntityFactory::TNT_BLOCK;

		EntityFactory::GetInstance()->CreateEntity(e, type);
		e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
		m_world->AddEntity(e);
		counter = 0;
			
	}

	m_world->Update(_dt);

	if (!m_world->IsAlive() && !m_gameOver)
	{
            Entity* e = m_world->GetEntity(m_gameOverHandle);
            auto TC = e->GetComponent<TextComponent>();

            GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
            m_gameOver = true;
            //SceneManager::GetInstance()->ChangeScene<GameOverScene>();
	}

        
        
}

void GameScene::UpdateFPS(float _dt)
{
        
        float fps = 1.0f / _dt;
        m_fpsString= "FPS: ";
        m_fpsString.append(std::to_string((int)fps));
        m_fpsString += " DT: ";
        m_fpsString.append(std::to_string(_dt));
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
	GraphicsManager::GetInstance()->ShowMouseCursor(false);
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
		}	

		delete m_world;
	}
}

void GameScene::Reset()
{
	/*	New Implementation	*/
	m_world = new World();
	m_world->AddSystem<InputSystem>();
	m_world->AddSystem<MovementSystem>();
	m_world->AddSystem<PhysicsSystem>();
	m_world->AddSystem<ModelSystem>();
	m_world->AddSystem<PadCollisionSystem>();
	//m_world->AddSystem<ProjectileSystem>();
	m_world->AddSystem<ScoreSystem>();
	m_world->AddSystem<AudioSystem>();
	m_world->AddSystem<CollectPowerUpSystem>();
	m_world->AddSystem<CollisionDeflectionSystem>();
	m_world->AddSystem<CollisionDamageSystem>();
	m_world->AddSystem<LoseLifeSystem>();
	m_world->AddSystem<RespawnBallSystem>();
	m_world->AddSystem<LightSystem>();
	m_world->AddSystem<EffectSystem>();
	m_world->AddSystem<BlockSystem>();
    m_world->AddSystem<TextSystem>();
	m_world->AddSystem<SpawnPowerUpSystem>();
	m_world->AddSystem<KillOnTouchSystem>();

	/*	New Implementation	*/
            
    m_gameOver = false;

    Entity* e;
    
    //PAUSE
    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    auto TC = e->GetComponent<TextComponent>();
    TC->Initialize(&m_pauseString, 0.5f-(m_pauseString.size()*8.0f)/1280.0f*1.5f, 0.5f, 3.f, VECTOR3(1,1,0), 30.0f);
    m_pauseHandle = e->GetId();
    m_world->AddEntity(e);
    
    //GAME OVER
    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
    TC->Initialize(&m_gameOverString, 0.5f-(m_gameOverString.size()*8.0f)/1280.0f*2.0f, 0.5f, 4.f, VECTOR3(1,0.7f,0.3f), 30.0f);
    m_gameOverHandle = e->GetId();
    m_world->AddEntity(e);

    
    e = m_world->GetEntity(m_pauseHandle);
    GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(e));
    
    //FPS COUNTER      
    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
    TC->Initialize(&m_fpsString, 0.0f, 0.0f, 2.f, VECTOR3(0.5,1,0.5), 20);
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

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -20, 0));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(10, 1, 1));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 0, 100));
	e->GetComponent<LightComponent>()->SetColor(VECTOR3(0.7f, 0.7f, 0.7f));
	e->GetComponent<LightComponent>()->SetRange(500);
	m_world->AddEntity(e);

	//e = m_world->CreateEntity();
	//EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::POINTLIGHT);
	//e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 0, -100));
	//e->GetComponent<LightComponent>()->SetColor(VECTOR3(0.7f, 0.7f, 0.7f));
	//e->GetComponent<LightComponent>()->SetRange(500);
	//m_world->AddEntity(e);

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
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(1.1f, 1, 1));
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -25, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::H_WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 29.5f, 0));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(1.1f, 1, 1));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-49.5f - e->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(49.5f + e->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();	
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLAYER);
	m_world->AddEntity(e);

        //PLAYER SCORE >>
        auto SC = e->GetComponent<ScoreComponent>();
        
        Entity* t = m_world->CreateEntity();
        EntityFactory::GetInstance()->CreateEntity(t, EntityFactory::TEXT);
        TC = t->GetComponent<TextComponent>();
        SC->SetString();
        m_world->AddEntity(t);
        
	TC->Initialize(SC->GetString(), 0.005f, 0.9f, 2.f, VECTOR3(0, 1, 1), 5);
        
        
	GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(t), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
	


	auto LC = e->GetComponent<LifeComponent>();

	Entity* t2 = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(t2, EntityFactory::TEXT);
	TC = t2->GetComponent<TextComponent>();

	TC->Initialize(LC->GetString(), 0.005f, 0.87f, 2.f, VECTOR3(0, 1, 1), 5);
	m_world->AddEntity(t2);
	GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(t2), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
	LC->SetString();
        
	//	Background
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLANE);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-53, -29, -5));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(110, 60, 1));
	m_world->AddEntity(e);

	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3(0, 1, 67));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(0.5f, 0.5f);

}