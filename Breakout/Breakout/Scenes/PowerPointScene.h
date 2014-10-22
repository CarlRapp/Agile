#ifndef POWERPOINTSCENE_H
#define POWERPOINTSCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../ComponentSystem/World.h"
#include "../ComponentSystem/Component/TextComponent.h"
#include "../ComponentSystem/EntityFactory.h"

using namespace SceneSystem;

class PowerPointScene : public Scene<PowerPointScene>
{
private:
	World* m_world;

	TextComponent *m_Start, *m_Options, *m_Exit;

	int	m_titleA;
	int	m_rowsA[11];

	float m_blockTimer;
	float m_elapsedTime;
	void CreatePlayField();

	void SceneA();
	void SceneB();

public:
	PowerPointScene();
	~PowerPointScene();

	void Initialize();
	void LoadContent();
	void Update(float _dt);
	void Render(float _dt);

	void OnActive();
	void OnInactive();
};


#endif