#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../ComponentSystem/World.h"
#include "../ComponentSystem/Component/TextComponent.h"

using namespace SceneSystem;

class MainMenuScene : public Scene<MainMenuScene>
{
private:
    World* m_world;
    int m_playID;
    int m_optionsID;
    int m_exitID;

	TextComponent *m_Start, *m_Options, *m_Exit;

	float m_blockTimer;
	void CreatePlayField();

public:
	MainMenuScene();
	~MainMenuScene();

	void Initialize();
	void LoadContent();
	void Update(float _dt);
	void Render(float _dt);

	void OnActive();
	void OnInactive();
};


#endif