#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "ISystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/PositionComponent.h"

class CollisionSystem : public System<CollisionSystem>
{
private:
public:
	CollisionSystem(World* _world);
	~CollisionSystem();

	void Update(float _dt);
};

#endif