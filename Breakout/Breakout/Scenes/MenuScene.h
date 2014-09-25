#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "../stdafx.h"
#include "../ComponentSystem/World.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"

using namespace SceneSystem;

class MenuScene : public Scene<MenuScene>
{
private:
	World* m_world;
public:
	MenuScene();
	~MenuScene();

	void Initialize();
	void LoadContent();
	void Update(float _dt);
	void Render();
};


#endif