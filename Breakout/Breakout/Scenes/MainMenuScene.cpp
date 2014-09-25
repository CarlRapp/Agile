#include "MainMenuScene.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "GameScene.h"

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
}

void MainMenuScene::OnActive()
{
	GraphicsManager* GM = GraphicsManager::GetInstance();
	GM->Add2DTexture(1337, "MainMenu.png", &m_x, &m_y, &m_width, &m_height);
	m_trollCounter = 0;
}
void MainMenuScene::OnInactive()
{
	GraphicsManager* GM = GraphicsManager::GetInstance();
	GM->Remove2DTexture(1337);
}

void MainMenuScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(27) == InputState::Pressed)
		SceneManager::GetInstance()->Quit();

	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
		SceneManager::GetInstance()->ChangeScene<GameScene>();
		

	m_x = sinf(100*m_trollCounter) * 0.02f;
	m_y = sinf(10*m_trollCounter) * 0.02f;

	m_trollCounter += _dt;
}

void MainMenuScene::Render()
{
	GraphicsManager::GetInstance()->Render();
}