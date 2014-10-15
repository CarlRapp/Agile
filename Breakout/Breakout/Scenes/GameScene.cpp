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
#define STATS_INC_SPEED     5
#define STATS_INC_SIZE      1
#define STATS_INC_DAMAGE    1
#define STATS_LIM_SPEED     200
#define STATS_LIM_SIZE      50
#define STATS_LIM_DAMAGE    500
#define STATS_INIT          30

float counter;
std::string m_fpsString= "FPS: ";
std::string m_pauseString= "GAME PAUSED";
std::string m_gameOverString= "GAME OVER";

GameScene::GameScene()
	: m_world(0), m_pauseBackground(0)
{
	printf("Game Scene created!\n");
}

GameScene::~GameScene()
{
	SafeDelete(m_world);
	SafeDelete(m_pauseBackground);
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

	std::vector<std::string> files;
	FileManager::GetInstance().GetFilesInDirectory(files, MODEL_ROOT);

	for (auto file : files)
	{
		file = file.substr(0, file.size() - 4);
		GraphicsManager::GetInstance()->GetIGraphics()->LoadModel(file);
	}
}

void GameScene::Update(float _dt)
{
        UpdateFPS(_dt);
    
        if(m_levelUp)
        {
            LevelUpMenu(_dt);
            return;
        }
        
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
            m_world->UpdateTextOnly(_dt);

            if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
                    SceneManager::GetInstance()->ChangeScene<MainMenuScene>();

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
		EntityFactory::GetInstance()->CreateEntity(e, RandomizeType());
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

EntityFactory::EntityType GameScene::RandomizeType(void)
{
	EntityFactory::EntityType type;

	const int smallRed		= 14;	// 14%
	const int smallGreen	= 28;	// 14%
	const int smallBlue		= 42;	// 14%
	const int bigRed		= 56;	// 14%
	const int bigGreen		= 70;	// 14%
	const int bigBlue		= 84;	// 14%
	const int indestruct	= 92;	// 8%
	const int tnt			= 100;	// 8%

	const int rnd = (rand() % (100 - 0)); // randomize between 0 and 100

	if (rnd >= 0 && rnd < smallRed)
		type = EntityFactory::STANDARD_BLOCK_RED;

	else if (rnd >= smallRed && rnd < smallGreen)
		type = EntityFactory::STANDARD_BLOCK_GREEN;

	else if (rnd >= smallGreen && rnd < smallBlue)
		type = EntityFactory::STANDARD_BLOCK_BLUE;

	else if (rnd >= smallBlue && rnd < bigRed)
		type = EntityFactory::STANDARD_BIG_RED;

	else if (rnd >= bigRed && rnd < bigGreen)
		type = EntityFactory::STANDARD_BIG_GREEN;

	else if (rnd >= bigGreen && rnd < bigBlue)
		type = EntityFactory::STANDARD_BIG_BLUE;

	else if (rnd >= bigBlue && rnd < indestruct)
		type = EntityFactory::INDESTRUCTIBLE_BLOCK;

	else if (rnd >= indestruct && rnd < tnt)
		type = EntityFactory::TNT_BLOCK;

	return type;
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
	GraphicsManager::GetInstance()->SetSky("snowcube1024");
	GraphicsManager::GetInstance()->ShowMouseCursor(false);
	Reset();
}
void GameScene::OnInactive()
{
	GraphicsManager::GetInstance()->ClearSky();
	GraphicsManager::GetInstance()->Remove2DTexture(GetMemoryID(m_pauseBackground));
	m_isPaused = false;
	if (m_world)
	{
		//GraphicsManager::GetInstance()->Clear();
		EntityMap::iterator eIT;
		for (eIT = m_world->GetAllEntities()->begin(); eIT != m_world->GetAllEntities()->end(); ++eIT)
		{
			eIT->second->SetInitialized(false);
			GraphicsManager::GetInstance()->RemoveObject(GetMemoryID(eIT->second));
			GraphicsManager::GetInstance()->RemovePointLight(GetMemoryID(eIT->second));
			GraphicsManager::GetInstance()->RemoveParticleEffect(GetMemoryID(eIT->second));
			
			GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(eIT->second));
		}

		
		SafeDelete(m_world);
	}
}

void GameScene::Reset()
{
    m_levelUp=0;
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
    TC->Initialize(&m_pauseString, 0.5f-(m_pauseString.size()*8.0f)/1280.0f*1.5f, 0.5f, 3.f, VECTOR3(1,1,0), 1.0f);
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
	//e = m_world->CreateEntity();
	//EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLANE);
	//e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-53, -29, -5));
	//e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(110, 60, 1));
	//m_world->AddEntity(e);

        
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(0.5f, 0.5f);

        //LEVEL UP STRINGS
        e = m_world->CreateEntity();
        EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
        TC = e->GetComponent<TextComponent>();
        TC->Initialize("LEVEL UP - SELECT STATS", 0.5f-(23*8.0f)/1280.0f*2.0f, 0.7f, 4.f, VECTOR3(0.7f,1.0f,0.7f), 10.0f);
        m_lvlUpHandle0 = e->GetId();
        m_world->AddEntity(e);
        
        e = m_world->CreateEntity();
        EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
        TC = e->GetComponent<TextComponent>();
        TC->Initialize("SPEED", 0.5f-(5*8.0f)/1280.0f*1.5f, 0.6f, 3.f, VECTOR3(0.7f,0.1f,0), 10.0f);
        m_lvlUpHandle1 = e->GetId();
        m_world->AddEntity(e);
        
        e = m_world->CreateEntity();
        EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
        TC = e->GetComponent<TextComponent>();
        TC->Initialize("SIZE", 0.5f-(4*8.0f)/1280.0f*1.5f, 0.5f, 3.f, VECTOR3(0.7f,0.1f,0), 10.0f);
        m_lvlUpHandle2 = e->GetId();
        m_world->AddEntity(e);
        
        e = m_world->CreateEntity();
        EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
        TC = e->GetComponent<TextComponent>();
        TC->Initialize("DAMAGE", 0.5f-(6*8.0f)/1280.0f*1.5f, 0.4f, 3.f, VECTOR3(0.7f,0.1f,0), 10.0f);
        m_lvlUpHandle3 = e->GetId();
        m_world->AddEntity(e);
        
        Entity* player = m_world->GetEntities<PlayerComponent>()->at(0);
        
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, -20, 0));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(player->GetComponent<PlayerComponent>()->m_size, 1, 1));
	m_world->AddEntity(e);
        
        m_playerID = e->GetId();
        
        // CAMERA --------------------------------------------------------------------------
	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3(0, 1, 67));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));
        //----------------------------------------------------------------------------------
        
        Update(0.01f);
        Render(0.01f);
        
        
        LevelUp(STATS_INIT,true);
        
}

void GameScene::LevelUp(int _lvlUp, bool _addStrings)
{
    m_levelUp = _lvlUp;
    
    int checkMaxLevel = 0;
    
    Entity* e = m_world->GetEntity(m_lvlUpHandle0);
    auto TC = e->GetComponent<TextComponent>();
    TC->m_color = VECTOR3(0.1f,1.0f,0.1f);
    
    std::string add = "LEVEL UP - SELECT STATS (";
    add += std::to_string(m_levelUp);
    add += ")";
    
    TC->m_x = 0.5f-(add.size()*8.0f)/1280.0f*TC->m_scale*0.5;
    TC->SetText(add);
    
    if(_addStrings)
        GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
    
    e = m_world->GetEntity(m_lvlUpHandle1);
    TC = e->GetComponent<TextComponent>();
    
    Entity* player = m_world->GetEntities<PlayerComponent>()->at(0);
    if(player->GetComponent<PlayerComponent>()->m_maxSpeedBall >= STATS_LIM_SPEED)
    {
        TC->m_color = VECTOR3(1.f,1.f,0.0f);
        checkMaxLevel++;
    }
    else
        TC->m_color = VECTOR3(1.0f,1.0f,1.0f);
        
    add = "<1> - SPEED - (";
    add += std::to_string((int)player->GetComponent<PlayerComponent>()->m_maxSpeedBall);
    add += "/";
    add += std::to_string(STATS_LIM_SPEED);
    add += ")";
            
    TC->m_x = 0.5f-(add.size()*8.0f)/1280.0f*TC->m_scale*0.5;
    TC->SetText(add);
    if(_addStrings)
        GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
    
    e = m_world->GetEntity(m_lvlUpHandle2);
    TC = e->GetComponent<TextComponent>();
    
    if(m_world->GetEntity(m_playerID)->GetComponent<ScaleComponent>()->GetScale().x >= STATS_LIM_SIZE)
    {
        TC->m_color = VECTOR3(1.f,1.f,0.0f);
        checkMaxLevel++;
    }
    else
        TC->m_color = VECTOR3(1.0f,1.0f,1.0f);
    
    auto SC = m_world->GetEntity(m_playerID)->GetComponent<ScaleComponent>();
    
    add = "<2> - SIZE - (";
    add += std::to_string((int)SC->GetScale().x);
    add += "/";
    add += std::to_string(STATS_LIM_SIZE);
    add += ")";
            
    TC->m_x = 0.5f-(add.size()*8.0f)/1280.0f*TC->m_scale*0.5;
    TC->SetText(add);
    if(_addStrings)
        GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
    
    e = m_world->GetEntity(m_lvlUpHandle3);
    TC = e->GetComponent<TextComponent>();
    
    if(player->GetComponent<PlayerComponent>()->m_damage >= STATS_LIM_DAMAGE)
    {
        TC->m_color = VECTOR3(1.f,1.f,0.0f);
        checkMaxLevel++;
    }
    else
        TC->m_color = VECTOR3(1.0f,1.0f,1.0f);
    
    player = m_world->GetEntities<PlayerComponent>()->at(0);
        
    add = "<3> - DAMAGE - (";
    add += std::to_string((int)player->GetComponent<PlayerComponent>()->m_damage);
    add += "/";
    add += std::to_string(STATS_LIM_DAMAGE);
    add += ")";
    
    TC->m_x = 0.5f-(add.size()*8.0f)/1280.0f*TC->m_scale*0.5;
    TC->SetText(add);
    
    if(_addStrings)
        GraphicsManager::GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
    
    if(checkMaxLevel >= 3)
    {
        m_playerIsMaxLevel = true;
    }
}

void GameScene::LevelUpMenu(float _dt)
{
    if(m_playerIsMaxLevel)
        m_levelUp = 0;
    
    //CHECK FOR BUTTON TO CHOOSE STATS
    if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(49) == InputState::Pressed)
    {
        //INCREASE SPEED
        Entity* player = m_world->GetEntities<PlayerComponent>()->at(0);
        
        if(player->GetComponent<PlayerComponent>()->m_maxSpeedBall >= STATS_LIM_SPEED)
        {
            return;
        }
        
        Entity* e = m_world->GetEntity(m_lvlUpHandle1);
        auto TC = e->GetComponent<TextComponent>();
        TC->m_color = VECTOR3(0,0.5f,1);
        
        player->GetComponent<PlayerComponent>()->m_maxSpeedBall += 1;
        player->GetComponent<PlayerComponent>()->m_minSpeedBall += 1;
        
        float maxSpeed = player->GetComponent<PlayerComponent>()->m_maxSpeedBall;
        float minSpeed = player->GetComponent<PlayerComponent>()->m_minSpeedBall;
        std::vector<Entity*>* ballList = m_world->GetEntities<BallComponent>();
        if(ballList)
            for (int n = 0; n != ballList->size(); ++n)
            {
                Entity* ball = ballList->at(n);
                if(ball->GetState() == Entity::ALIVE)
                {
                    ball->GetComponent<CollisionStatsComponent>()->SetMaxSpeed(maxSpeed);
                    ball->GetComponent<CollisionStatsComponent>()->SetMinSpeed(minSpeed);
                }
            }
        
        
        m_levelUp--;
    }
    else if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(50) == InputState::Pressed)
    {
        //INCREASE SIZE
        
        if(m_world->GetEntity(m_playerID)->GetComponent<ScaleComponent>()->GetScale().x >= STATS_LIM_SIZE)
        {
            return;
        }
        
        Entity* e = m_world->GetEntity(m_lvlUpHandle2);
        auto TC = e->GetComponent<TextComponent>();
        TC->m_color = VECTOR3(0,0.5f,1);
        
        e = m_world->GetEntity(m_playerID);
        
        Entity* player = m_world->GetEntities<PlayerComponent>()->at(0);
        player->GetComponent<PlayerComponent>()->m_size += STATS_INC_SIZE;
        
        e->SetState(Entity::ENTITY_STATE::SOON_DEAD);
                
        auto PC = m_world->GetEntity(m_playerID)->GetComponent<PositionComponent>();
        
        VECTOR3 lastPos = PC->GetPosition();
        
        e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PAD);
	e->GetComponent<PositionComponent>()->SetPosition(lastPos);
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(player->GetComponent<PlayerComponent>()->m_size, 1, 1));
	m_world->AddEntity(e);
        
        m_playerID = e->GetId();
        
        m_levelUp--;
    }
    else if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(51) == InputState::Pressed)
    {
        //INCREASE DAMAGE
        Entity* player = m_world->GetEntities<PlayerComponent>()->at(0);
        
        if(player->GetComponent<PlayerComponent>()->m_damage >= STATS_LIM_DAMAGE)
        {
            return;
        }
        
        Entity* e = m_world->GetEntity(m_lvlUpHandle3);
        auto TC = e->GetComponent<TextComponent>();  
        TC->m_color = VECTOR3(0,0.5f,1);
        
        player->GetComponent<PlayerComponent>()->m_damage += 1;
        
        float damage = player->GetComponent<PlayerComponent>()->m_damage;
        
        std::vector<Entity*>* ballList = m_world->GetEntities<BallComponent>();
        if(ballList)
            for (int n = 0; n != ballList->size(); ++n)
            {
                Entity* ball = ballList->at(n);
                if(ball->GetState() == Entity::ALIVE)
                {
                   ball->GetComponent<DamageComponent>()->m_damage = damage;
                }
            }
        
        m_levelUp--;
    }
    else if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
        m_levelUp = 0;
    else if(!m_playerIsMaxLevel)
    {
        m_world->UpdateTextOnly(_dt);
        return;
    }
    
    //REMOVE LEVEL UP STRINGS IF NO MORE LEVELS
    if(!m_levelUp)
    {
        Entity* e = m_world->GetEntity(m_lvlUpHandle0);
        GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(e));

        e = m_world->GetEntity(m_lvlUpHandle1);
        GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(e));

        e = m_world->GetEntity(m_lvlUpHandle2);
        GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(e));

        e = m_world->GetEntity(m_lvlUpHandle3);
        GraphicsManager::GetInstance()->RemoveTextObject(GetMemoryID(e));
    }
    else
         LevelUp(m_levelUp,false);

    
    
    m_world->UpdateTextOnly(_dt);
}