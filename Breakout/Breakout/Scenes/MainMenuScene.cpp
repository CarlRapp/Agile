#include "MainMenuScene.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "GameScene.h"
#include "../ComponentSystem/Component/TextComponent.h"

#pragma region System includes
#include "../ComponentSystem/System/BlockSystem.h"
#include "../ComponentSystem/System/TextSystem.h"
#pragma endregion



MainMenuScene::MainMenuScene()
{
	printf("Main Menu created!\n");
}

MainMenuScene::~MainMenuScene()
{

}

void MainMenuScene::Initialize()
{
	printf("Initialized (Main Menu)\n");
}

void MainMenuScene::LoadContent()
{
	printf("Loading Content (Main Menu)\n");

    m_stringPlay        = "--START--";
    m_stringOptions     = "--OPTIONS--";
    m_stringExit        = "--EXIT--";
        

        //GraphicsManager::GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_exitID);
}

void MainMenuScene::OnActive()
{
	m_world = new World();

	CreatePlayField();

    GraphicsManager* GM = GraphicsManager::GetInstance();
    
    Entity* e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    auto TC = e->GetComponent<TextComponent>();
    TC->Initialize(&m_stringPlay, 0.05f, 0.9f, 2.f, VECTOR3(0,1,0), 20.0f);
    m_world->AddEntity(e);
    m_playID = e->GetId();
   // GM->GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_playID);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
	TC->Initialize(&m_stringOptions, 0.05f, 0.85f, 2.f, VECTOR3(0, 1, 0), 20.0f);
    //TC->Initialize(&m_stringOptions,2.f,0x1904 ,100,120);
    m_world->AddEntity(e);
    m_optionsID = e->GetId();
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
	TC->Initialize(&m_stringExit, 0.05f, 0.8f, 2.f, VECTOR3(0, 1, 0), 20.0f);
    //TC->Initialize(&m_stringExit,2.f,0x1904 ,100,140);
    m_world->AddEntity(e);
    m_exitID = e->GetId();
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	
}
void MainMenuScene::OnInactive()
{
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

void MainMenuScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
		SceneManager::GetInstance()->Quit();

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
	{
		SceneManager::GetInstance()->ChangeScene<GameScene>();
		return;
	}
		

	m_world->Update(_dt);

	m_blockTimer += _dt;
	if (m_blockTimer > .50f)
	{
		m_blockTimer = 0;
		Entity* e;
		e = m_world->CreateEntity();
		if (!e)
			return;
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
	}
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
	m_world->AddSystem<BlockSystem>();
        m_world->AddSystem<TextSystem>();
	/*	New Implementation	*/

	//FPS COUNTER
	Entity* e;

	for (int i = 0; i < 0; ++i)
	{
		e = m_world->CreateEntity();
		int rnd = (rand() % (3 - 0));
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::STANDARD_HORIZONTAL_RECTANGLE);
		e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
		m_world->AddEntity(e);
	}

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
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PLANE);
	e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(-53, -29, -5));
	e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(110, 60, 1));
	m_world->AddEntity(e);

	GraphicsManager::GetInstance()->GetICamera()->SetPosition(VECTOR3(0, 1, 67));
	GraphicsManager::GetInstance()->GetICamera()->SetForward(VECTOR3(0, 0, -1));
	InputManager::GetInstance()->getInputDevices()->GetMouse()->SetMousePosition(500, 300);
}