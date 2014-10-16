#include "MainMenuScene.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "GameScene.h"
#include "../ComponentSystem/Component/TextComponent.h"

#pragma region System includes
#include "../ComponentSystem/System/BlockSystem.h"
#include "../ComponentSystem/System/TextSystem.h"
#include "../ComponentSystem/System/BlockSpawnSystem.h"
#pragma endregion



MainMenuScene::MainMenuScene()
	: m_world(0), m_Start(0), m_Options(0), m_Exit(0)
{
	printf("Main Menu created!\n");
}

MainMenuScene::~MainMenuScene()
{
	SafeDelete(m_world);
	m_Start		= 0;
	m_Options	= 0;
	m_Exit		= 0;
}

void MainMenuScene::Initialize()
{
	printf("Initialized (Main Menu)\n");
}

void MainMenuScene::LoadContent()
{
	printf("Loading Content (Main Menu)\n");
        

        //GraphicsManager::GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_exitID);
}

void MainMenuScene::OnActive()
{
	m_world = new World();

	GraphicsManager::GetInstance()->GetICamera()->AddShake(0, 20, 40, 0.3f, -1);

	CreatePlayField();

    GraphicsManager* GM = GraphicsManager::GetInstance();
	GM->SetSky("space2");
	GM->ShowMouseCursor(true);

    Entity* e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    auto TC = e->GetComponent<TextComponent>();
	m_Start = TC;
	TC->Initialize("--START--", 0.05f, 0.9f, 4.f, VECTOR3(0, 1, 0), 20.0f);
    m_world->AddEntity(e);
    m_playID = e->GetId();
   // GM->GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_playID);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
	m_Options = TC;
	TC->Initialize("--OPTIONS--", 0.05f, 0.85f, 4.f, VECTOR3(0, 1, 0), 20.0f);
    //TC->Initialize(&m_stringOptions,2.f,0x1904 ,100,120);
    m_world->AddEntity(e);
    m_optionsID = e->GetId();
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
	m_Exit = TC;
	TC->Initialize("--EXIT--", 0.05f, 0.8f, 4.f, VECTOR3(0, 1, 0), 20.0f);
    m_world->AddEntity(e);
    m_exitID = e->GetId();
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

    float highscoreY = 0.9f;
        
    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
    TC->Initialize("-HIGHSCORES-", 0.5f-(12*8.0f)/1280.0f*1.5f, highscoreY, 3.f, VECTOR3(1, 1, 1), 20.0f);

    m_world->AddEntity(e);
    GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

        
    vector<HighScore> v = FileManager::GetInstance().LoadHighScores(GetFile("highscores.txt",HIGHSCORE_ROOT));
    
    
    for(int i =0;i < v.size();i++)
    {
        e = m_world->CreateEntity();
        EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
        TC = e->GetComponent<TextComponent>();
        std::string score = v.at(i).m_name;
        score += " ";
        score += std::to_string(v.at(i).m_score);
        TC->Initialize(score, 0.5f-(score.size()*8.0f)/1280.0f, highscoreY-(i+1)*0.05f, 2.f, VECTOR3(1, 1, 1), 20.0f);

        m_world->AddEntity(e);

        GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
        
        if(i > 10)
            break;
    }
	
}
void MainMenuScene::OnInactive()
{
	GraphicsManager::GetInstance()->GetICamera()->RemoveShake(0);

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

void MainMenuScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
		SceneManager::GetInstance()->Quit();

	GraphicsManager::GetInstance()->GetICamera()->Update(_dt);

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
	{
		SceneManager::GetInstance()->ChangeScene<GameScene>();
		return;
	}
	
	float x = InputManager::GetInstance()->getInputDevices()->GetMouse()->GetX();
	float y = InputManager::GetInstance()->getInputDevices()->GetMouse()->GetY();

	bool pressed = InputManager::GetInstance()->getInputDevices()->GetMouse()->GetButtonState(MouseButton::LeftMB) == InputState::Pressed;

	//START
	if (m_Start->GetBoundingBox2D().Intersects(VECTOR2(x, y)))
	{
		m_Start->m_color = VECTOR3(1, 0, 0);

		if (pressed)
		{
			SceneManager::GetInstance()->ChangeScene<GameScene>();
			return;
		}
	}
	else
		m_Start->m_color = VECTOR3(0, 1, 0);
	//OPTIONS
	if (m_Options->GetBoundingBox2D().Intersects(VECTOR2(x, y)))
	{
		m_Options->m_color = VECTOR3(1, 0, 0);
	}
	else
		m_Options->m_color = VECTOR3(0, 1, 0);
	//EXIT
	if (m_Exit->GetBoundingBox2D().Intersects(VECTOR2(x, y)))
	{
		m_Exit->m_color = VECTOR3(1, 0, 0);
		if (pressed)
		{			
			SceneManager::GetInstance()->Quit();
		}
	}
	else
		m_Exit->m_color = VECTOR3(0, 1, 0);
	


	m_world->Update(_dt);
}

void MainMenuScene::Render(float _dt)
{
	GraphicsManager::GetInstance()->Render(_dt);
}

void MainMenuScene::CreatePlayField()
{
	/*	New Implementation	*/
	m_world->AddSystem<InputSystem>();
	m_world->AddSystem<PhysicsSystem>();
	m_world->AddSystem<ModelSystem>();
	m_world->AddSystem<AudioSystem>();
	m_world->AddSystem<CollisionDeflectionSystem>();
	m_world->AddSystem<CollisionDamageSystem>();
	m_world->AddSystem<LightSystem>();
	m_world->AddSystem<EffectSystem>();
	m_world->AddSystem<BlockSpawnSystem>();
	m_world->AddSystem<BlockSystem>();
    m_world->AddSystem<TextSystem>();

	BlockSpawnSystem* blockSystem = m_world->GetSystem<BlockSpawnSystem>();
	BlockPool*	easyBlocks = new BlockPool();
	easyBlocks->SetPoolChance(60);
	easyBlocks->AddBlockToPool(BlockType::BLACK_SMALL);
	easyBlocks->AddBlockToPool(BlockType::RED_SMALL);
	easyBlocks->AddBlockToPool(BlockType::GREEN_SMALL);
	easyBlocks->AddBlockToPool(BlockType::BLUE_SMALL);
	blockSystem->AddBlockPool(easyBlocks);
	BlockPool*	mediumBlocks = new BlockPool();
	mediumBlocks->SetPoolChance(30);
	mediumBlocks->AddBlockToPool(BlockType::RED_MEDIUM);
	mediumBlocks->AddBlockToPool(BlockType::GREEN_MEDIUM);
	mediumBlocks->AddBlockToPool(BlockType::BLUE_MEDIUM);
	blockSystem->AddBlockPool(mediumBlocks);
	BlockPool*	rareBlocks = new BlockPool();
	rareBlocks->SetPoolChance(10);
	rareBlocks->AddBlockToPool(BlockType::TNT_SMALL);
	blockSystem->AddBlockPool(rareBlocks);

	/*	New Implementation	*/

	//FPS COUNTER
	Entity* e;

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BALL);
	e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(rand() % 20 - 10, 40, 0);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 0, 0));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(0.8f, 0.8f, 0.8f));
	
	e->GetComponent<EffectComponent>()->m_effects.OnAdded = EffectFlags::TRAIL;
	
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
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 1, 1));
	e->RemoveComponent<AudioComponent>();
	e->RemoveComponent<DamageComponent>();
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::H_WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(0, 29.5f, 0));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 1, 1));

	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-49.5f - e->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, 0, 0));
	m_world->AddEntity(e);

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::WALL);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(49.5f + e->GetComponent<ScaleComponent>()->GetScale().x * 0.5f, 0, 0));
	m_world->AddEntity(e);


	//PLAYER SCORE <<

	//	Background
	//e = m_world->CreateEntity();
	//EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLANE);
	//e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-53, -29, -5));
	//e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(110, 60, 1));
	//m_world->AddEntity(e);

	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3(0, 1, 67));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(0.5f, 0.5f);
}