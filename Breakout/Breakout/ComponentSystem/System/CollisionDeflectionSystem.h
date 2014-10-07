#ifndef COLLISIONDEFLECTIONSYSTEM_H
#define COLLISIONDEFLECTIONSYSTEM_H

#include <vector>

#include "ISystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/VelocityComponent.h"
#include "../Component/DeflectionComponent.h"

class CollisionDeflectionSystem : public System<CollisionDeflectionSystem>
{
public:
	CollisionDeflectionSystem(World* _world);
	~CollisionDeflectionSystem();

	void Update(float _dt);
};
#endif