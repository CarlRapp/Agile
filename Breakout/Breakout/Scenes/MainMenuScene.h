#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../ComponentSystem/World.h"

using namespace SceneSystem;

class MainMenuScene : public Scene<MainMenuScene>
{
private:
    World* m_world;
    std::string m_stringPlay;
    std::string m_stringOptions;
    std::string m_stringExit;
    int m_playID;
    int m_optionsID;
    int m_exitID;
    void LoadText();
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