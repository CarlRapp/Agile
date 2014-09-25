#include "MenuScene.h"

MenuScene::MenuScene()
{
	printf("Game Scene created!\n");
}

MenuScene::~MenuScene()
{

}

void MenuScene::Initialize()
{
	printf("Initialized (Game Scene)\n");

	/*	New Implementation	*/
	m_world = new World();
}

void MenuScene::LoadContent()
{
	printf("Loading Content (Game Scene)\n");
}

void MenuScene::Update(float _dt)
{
	m_world->Update(_dt);
}

void MenuScene::Render()
{
	GraphicsManager::GetInstance()->Render();
}