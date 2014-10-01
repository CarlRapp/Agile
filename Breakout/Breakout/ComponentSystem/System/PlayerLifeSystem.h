#ifndef PLAYERLIFESYSTEM_H
#define PLAYERLIFESYSTEM_H

#include "ISystem.h"
#include "../Component/PlayerLifeComponent.h"

class PlayerLifeSystem : public System<PlayerLifeSystem>
{
private:
	void OnEntityAdded(Entity* _e);
	void OnEntityRemove(Entity* _e);

public:

	PlayerLifeSystem(World* _world);
	~PlayerLifeSystem();

	void Update(float _dt);
};

#endif