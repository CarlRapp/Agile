#include "PowerPointScene.h"
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



PowerPointScene::PowerPointScene()
: m_world(0), m_Start(0), m_Options(0), m_Exit(0)
{
	printf("Main Menu created!\n");
}

PowerPointScene::~PowerPointScene()
{
	SafeDelete(m_world);
	m_Start = 0;
	m_Options = 0;
	m_Exit = 0;
}

void PowerPointScene::Initialize()
{
	printf("Initialized (Main Menu)\n");
}

void PowerPointScene::LoadContent()
{
	printf("Loading Content (Main Menu)\n");


	//GraphicsManager::GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_exitID);
}

void PowerPointScene::OnActive()
{
	m_world = new World();
	CreatePlayField();
	m_elapsedTime = 0.0f;
	GraphicsManager::GetInstance()->GetICamera()->AddShake(0, 20, 40, 0.3f, -1);

	GraphicsManager* GM = GraphicsManager::GetInstance();
	GraphicsManager::GetInstance()->SetSky("BackGroundCube");
	//GM->ShowMouseCursor(true);

	m_titleA = 0;
	for (int i = 0; i < 11; ++i)
		m_rowsA[i] = 0;
}
void PowerPointScene::SceneA()
{
	GraphicsManager* GM = GraphicsManager::GetInstance();
	Entity* e;

	GM->GetInstance()->RemoveTextObject(m_titleA);
	for (int i = 0; i < 11; ++i)
	{
		GM->GetInstance()->RemoveTextObject(m_rowsA[i]);
	}

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	auto TC = e->GetComponent<TextComponent>();
	TC->Initialize("Intro - Original Ideas", 0.05f, 0.90f, 4.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_titleA = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	float	startY = 0.83f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Dynamic Level", 0.07f, startY, 3.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[0] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Builds the level while you are playing", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[1] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Infinte levels", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[2] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);


	startY -= 0.08f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Powerups", 0.07f, startY, 3.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[3] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Temporary boosts", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[4] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);


	startY -= 0.08f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Level system", 0.07f, startY, 3.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[5] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Permanent boosts during the game", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[6] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Points gives experience", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[7] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.08f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Graphical Elements", 0.07f, startY, 3.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[8] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("3D but with 2D logic", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[9] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Camera effects", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[10] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
}

void PowerPointScene::SceneB()
{
	GraphicsManager* GM = GraphicsManager::GetInstance();
	Entity* e;

	GM->GetInstance()->RemoveTextObject(m_titleA);
	for (int i = 0; i < 11; ++i)
	{
		GM->GetInstance()->RemoveTextObject(m_rowsA[i]);
	}

	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	auto TC = e->GetComponent<TextComponent>();
	TC->Initialize("Intro - Goals", 0.05f, 0.90f, 4.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_titleA = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	float	startY = 0.83f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Crossplatform", 0.07f, startY, 3.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[0] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Sync between Windows and Linux", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[1] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Hide all platform dependency behind interfaces", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[2] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);


	startY -= 0.08f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Performance", 0.07f, startY, 3.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[3] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("60 fps", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[4] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);


	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Intel i7 2.80GHz          4GB RAM   Radeon HD 5800 Series", 0.10f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[5] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("AMD Athlon II X2 2.2GHz   4GB RAM   Radeon HD 5650 Series", 0.10f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[6] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.08f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Modularity in the game", 0.07f, startY, 3.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[7] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);

	startY -= 0.03f;
	e = m_world->CreateEntity();
	EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
	TC = e->GetComponent<TextComponent>();
	TC->Initialize("Entity Component System", 0.0885f, startY, 2.f, VECTOR3(1, 1, 1), 20.0f);
	m_world->AddEntity(e);
	m_rowsA[8] = GetMemoryID(e);
	GM->GetInstance()->AddTextObject(GetMemoryID(e), TC->m_text, &TC->m_x, &TC->m_y, &TC->m_scale, &TC->m_color, &TC->m_effect);
}


void PowerPointScene::OnInactive()
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
void PowerPointScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
		SceneManager::GetInstance()->Quit();
        
        if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
        {
		SceneManager::GetInstance()->ChangeScene<MainMenuScene>();
                return;
        }

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('1') == InputState::Pressed)
		SceneA();
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState('2') == InputState::Pressed)
		SceneB();

	VECTOR3 pos = GraphicsManager::GetInstance()->GetICamera()->GetPosition();
	m_elapsedTime += _dt * 0.25f;
	pos.y = 20;
	pos.x = 0 + 100 * sinf(m_elapsedTime);
	pos.z = 100 * cosf(m_elapsedTime);

	GraphicsManager::GetInstance()->GetICamera()->SetPosition(pos);
	GraphicsManager::GetInstance()->GetICamera()->SetLookAt(VECTOR3(0, 0, 0));

	m_blockTimer += _dt;
	if (m_blockTimer > 0.1f)
	{
		m_world->GetSystem<BlockSpawnSystem>()->ForceSpawn(1);
		m_blockTimer = 0.0f;
	}

	m_world->Update(_dt);
	GraphicsManager::GetInstance()->GetICamera()->SetLookAt(VECTOR3(0, 0, 0));
}

void PowerPointScene::Render(float _dt)
{
	GraphicsManager::GetInstance()->Render(_dt);
}

void PowerPointScene::CreatePlayField()
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