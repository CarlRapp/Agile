#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
using namespace SceneSystem;

class MainMenuScene : public Scene<MainMenuScene>
{
private:
	float m_x, m_y;
	float m_width, m_height;
	float m_trollCounter;
public:
	MainMenuScene();
	~MainMenuScene();

	void Initialize();
	void LoadContent();
	void Update(float _dt);
	void Render();

	void OnActive();
	void OnInactive();
};


#endif