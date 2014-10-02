#ifndef RESPAWNBALLSYSTEM_H
#define RESPAWNBALLSYSTEM_H

#include "ISystem.h"
#include "../Component/LoseLifeComponent.h"
#include "../Component/SpawnEntityComponent.h"
#include "../Component/MouseInputComponent.h"
#include "../Component/PositionComponent.h"
#include "../EntityFactory.h"

class RespawnBallSystem : public System<RespawnBallSystem>
{
private:
	int m_numBallsLeft;

public:
	RespawnBallSystem(World* _world);
	~RespawnBallSystem();

	void Update(float _dt);

	void OnEntityAdded(Entity* _e);
	void OnEntityRemoved(Entity* _e);
};

#endif