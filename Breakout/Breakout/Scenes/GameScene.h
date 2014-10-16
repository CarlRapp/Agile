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
#include "../ComponentSystem/System/CollisionDeflectionSystem.h"
#include "../ComponentSystem/System/AudioSystem.h"
#include "../ComponentSystem/System/ScoreSystem.h"
#include "../ComponentSystem/System/LightSystem.h"
#include "../ComponentSystem/System/EffectSystem.h"
#include "../ComponentSystem/System/RespawnBallSystem.h"
#include "../Input/InputManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../ComponentSystem/EntityFactory.h"
using namespace SceneSystem;

class GameScene : public Scene<GameScene>
{
private:

	void Reset(void);

	World* m_world;

	// Pause Screen settings
	Texture2DData*	m_pauseBackground;
	bool m_isPaused= false;
    int m_fpsCounterID= 0;
    int m_pauseHandle= 0;
    int m_gameOverHandle= 0;
    bool m_gameOver = false;

        
        int m_lvlUpHandleMAIN= 0;
        int m_lvlUpHandleSPEED= 0;
        int m_lvlUpHandleSIZE= 0;
        int m_lvlUpHandleDAMAGE = 0;
        int m_lvlUpHandleAUTO = 0;
        
        
        int m_levelUp = 0;
        int m_playerID = 0;
        bool m_playerIsMaxLevel = false;
        
        void SkillSpeed();
        void SkillSize();
        void SkillDamage();
public:
	GameScene();
	~GameScene();

	void Initialize();
	void LoadContent();
	void Update(float _dt);
	void Render(float _dt);

	void OnActive();
	void OnInactive();
        void UpdateFPS(float _dt);
        void LevelUp(int _lvlUp, bool _addStrings);
        void LevelUpMenu(float _dt);
};


#endif