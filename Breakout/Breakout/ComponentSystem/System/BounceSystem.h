#ifndef BOUNCESYSTEMSYSTEM_H
#define BOUNCESYSTEMSYSTEM_H

#include <vector>

#include "ISystem.h"
#include "../Component/BounceComponent.h"
#include "../Component/DeflectionComponent.h"
#include "../Component/CollisionComponent.h"

class BounceSystem : public System<BounceSystem>
{
public:

	BounceSystem(World* _world);
	~BounceSystem();

	void Update(float _dt);
};
#endif