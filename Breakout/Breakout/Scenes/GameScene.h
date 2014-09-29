#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../stdafx.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../ComponentSystem/World.h"
#include "../ComponentSystem/System/ModelSystem.h"
#include "../ComponentSystem/System/MovementSystem.h"
#include "../ComponentSystem/System/InputSystem.h"
#include "../ComponentSystem/System/PhysicsSystem.h"
#include "../ComponentSystem/System/ProjectileSystem.h"
#include "../ComponentSystem/System/CollisionDamageSystem.h"
#include "../ComponentSystem/System/BounceSystem.h"
#include "../ComponentSystem/System/AudioSystem.h"
#include "../ComponentSystem/System/ScoreSystem.h"
#include "../ComponentSystem/System/LightSystem.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../ComponentSystem/EntityFactory.h"
using namespace SceneSystem;

class GameScene : public Scene<GameScene>
{
private:

	void Reset(void);

	World* m_world;
	Entity* m_ball;
public:
	GameScene();
	~GameScene();

	void Initialize();
	void LoadContent();
	void Update(float _dt);
	void Render();

	void OnActive();
	void OnInactive();
};


#endif