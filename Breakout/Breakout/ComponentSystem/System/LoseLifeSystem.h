#ifndef LOSELIFESYSTEM_H
#define LOSELIFESYSTEM_H

#include "ISystem.h"
#include "../Component/LoseLifeComponent.h"
#include "../Component/LifeComponent.h"
#include "../Component/PlayerComponent.h"

class LoseLifeSystem : public System<LoseLifeSystem>
{
private:
	void OnEntityAdded(Entity* _e);
	void OnEntityRemoved(Entity* _e);

public:

	LoseLifeSystem(World* _world);
	~LoseLifeSystem();
};

#endif