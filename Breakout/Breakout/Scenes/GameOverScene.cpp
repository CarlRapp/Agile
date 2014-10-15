#include "GameOverScene.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "MainMenuScene.h"

GameOverScene::GameOverScene()
	: m_background(0)
{
	printf("Main Menu created!\n");
}

GameOverScene::~GameOverScene()
{
	SafeDelete(m_background);

}

void GameOverScene::Initialize()
{
	printf("Initialized (Main Menu)\n");
}

void GameOverScene::LoadContent()
{
	printf("Loading Content (Main Menu)\n");

	m_background = new Texture2DData();
	m_background->m_positionX = 0;
	m_background->m_positionY = 0;
	m_background->m_imageWidth = 1;
	m_background->m_imageHeight = 1;
	m_background->m_textureName = "GameOver.png";
}

void GameOverScene::OnActive()
{
	GraphicsManager* GM = GraphicsManager::GetInstance();
	GM->ShowMouseCursor(true);
	GM->Add2DTexture(
		GetMemoryID(m_background), m_background->m_textureName,
		&m_background->m_positionX, &m_background->m_positionY,
		&m_background->m_imageWidth, &m_background->m_imageHeight
		);
}
void GameOverScene::OnInactive()
{
	GraphicsManager* GM = GraphicsManager::GetInstance();
	GM->Remove2DTexture(GetMemoryID(m_background));
}

void GameOverScene::Update(float _dt)
{
	if (InputManager::GetInstance()->getInputDevices()->GetKeyboard()->GetKeyState(13) == InputState::Pressed)
		SceneManager::GetInstance()->ChangeScene<MainMenuScene>();
}

void GameOverScene::Render(float _dt)
{
	GraphicsManager::GetInstance()->Render(_dt);
}