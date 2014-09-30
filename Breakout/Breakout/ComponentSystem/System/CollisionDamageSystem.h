#ifndef COLLISIONDAMAGESYSTEM_H
#define COLLISIONDAMAGESYSTEM_H

#include <vector>

#include "ISystem.h"
#include "../Component/DamageComponent.h"
#include "../Component/HealthComponent.h"
#include "../Component/CollisionComponent.h"
#include "../Component/ModelComponent.h"

class CollisionDamageSystem : public System<CollisionDamageSystem>
{
private:
	inline void DealDamage(DamageComponent& _damage, HealthComponent& _health) const;

public:

	CollisionDamageSystem(World* _world);
	~CollisionDamageSystem();

	void Update(float _dt);
};
#endif