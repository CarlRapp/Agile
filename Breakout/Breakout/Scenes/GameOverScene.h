#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Graphics/GraphicsManager.h"
using namespace SceneSystem;

class GameOverScene : public Scene<GameOverScene>
{
private:
	Texture2DData*	m_background;
public:
	GameOverScene();
	~GameOverScene();

	void Initialize();
	void LoadContent();
	void Update(float _dt);
	void Render(float _dt);

	void OnActive();
	void OnInactive();
};


#endif