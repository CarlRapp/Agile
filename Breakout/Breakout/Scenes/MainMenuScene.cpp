#include "MainMenuScene.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "GameScene.h"
#include "../ComponentSystem/Component/TextComponent.h"

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
	m_x = 0;
	m_y = 0;
	m_width = 1.f;
	m_height = 1.f;
	m_trollCounter = 0;
        
        m_world = new World();
        
        m_stringPlay        = "--START--";
        m_stringOptions     = "--OPTIONS--";
        m_stringExit        = "--EXIT--";
        

        //GraphicsManager::GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_exitID);
}

void MainMenuScene::LoadText()
{
    Entity* e = m_world->GetEntity(m_playID);
    auto TC = e->GetComponent<TextComponent>();
    GraphicsManager::GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_playID);
    
    e = m_world->GetEntity(m_optionsID);
    TC = e->GetComponent<TextComponent>();
    GraphicsManager::GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_optionsID);
    
    e = m_world->GetEntity(m_exitID);
    TC = e->GetComponent<TextComponent>();
    GraphicsManager::GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_exitID);
}

void MainMenuScene::OnActive()
{
    GraphicsManager* GM = GraphicsManager::GetInstance();
    
    Entity* e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    auto TC = e->GetComponent<TextComponent>();
    TC->Initialize(&m_stringPlay,2.f,0x1904 ,0.1f,0.6f);
    m_world->AddEntity(e);
    m_playID = e->GetId();
    GM->GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_playID);

    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
    TC->Initialize(&m_stringOptions,2.f,0x1904 ,0.1f,0.5f);
    m_world->AddEntity(e);
    m_optionsID = e->GetId();
    GM->GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_optionsID);

    e = m_world->CreateEntity();
    EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::TEXT);
    TC = e->GetComponent<TextComponent>();
    TC->Initialize(&m_stringExit,2.f,0x1904 ,0.1f,0.4f);
    m_world->AddEntity(e);
    m_exitID = e->GetId();
    GM->GetInstance()->AddTextObject(TC->m_text,&TC->m_scale,&TC->m_color,&TC->m_x,&TC->m_y,m_exitID);

    GM->Add2DTexture(1337, "PLANE.png", &m_x, &m_y, &m_width, &m_height);
    m_trollCounter = 0;
}
void MainMenuScene::OnInactive()
{
            
    EntityMap::iterator eIT;
    for (eIT = m_world->GetAllEntities()->begin(); eIT != m_world->GetAllEntities()->end(); ++eIT)
    {
        if(eIT->second->HasComponent<TextComponent>())
        {
            GraphicsManager::GetInstance()->RemoveTextObject(eIT->second->GetId());
        }
    }	
            
    GraphicsManager* GM = GraphicsManager::GetInstance();
    GM->Remove2DTexture(1337);
}

void MainMenuScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
		SceneManager::GetInstance()->Quit();

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
		SceneManager::GetInstance()->ChangeScene<GameScene>();
		

	//m_x = sinf(4*m_trollCounter) * 0.2f;
	//m_y = sinf(2*m_trollCounter) * 0.2f;

	m_trollCounter += _dt;
}

void MainMenuScene::Render(float _dt)
{
	GraphicsManager::GetInstance()->Render();
}