#ifndef COLLECTPOWERUPSYSTEM_H
#define COLLECTPOWERUPSYSTEM_H
#include "ISystem.h"
#include <vector>

class CollectPowerUpSystem : public System<CollectPowerUpSystem>
{
private:

public:
	CollectPowerUpSystem(World* _world);
	~CollectPowerUpSystem();

	void Update(float _dt);
};



#endif