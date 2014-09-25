#ifndef KILLOBJECTSYSTEM_H
#define KILLOBJECTSYSTEM_H

#include <vector>

#include "ISystem.h"
#include "../Component/DamageComponent.h"
#include "../Component/HealthComponent.h"
#include "../Component/CollisionComponent.h"

class KillObjectSystem : public System<KillObjectSystem>
{
private:
public:

	KillObjectSystem(World* _world);
	~KillObjectSystem();

	void Update(float _dt);
};
#endif