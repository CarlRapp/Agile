#include "GameScene.h"

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
}

void GameScene::LoadContent()
{
	printf("Loading Content (Game Scene)\n");
}

void GameScene::Update(float _dt)
{
	m_world->Update(_dt);
}

void GameScene::Render()
{
	GraphicsManager::GetInstance()->Render();
}