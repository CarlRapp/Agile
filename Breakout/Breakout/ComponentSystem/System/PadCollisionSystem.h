#ifndef PADCOLLISIONSYSTEM_H
#define PADCOLLISIONSYSTEM_H

#include "ISystem.h"
#include "../Component/CollisionComponent.h"

class PadCollisionSystem : public System<PadCollisionSystem>
{
private:

public:
	PadCollisionSystem(World* _world);
	~PadCollisionSystem();

	void Update(float _dt);
};


#endif
