#ifndef SPAWNPOWERUPSYSTEM_H
#define SPAWNPOWERUPSYSTEM_H
#include "ISystem.h"


class SpawnPowerUpSystem : public System<SpawnPowerUpSystem>
{
private:
	enum PowerUpType
	{
		MULTIBALL,
		SHOOTLASER
	};
	void OnEntityRemoved(Entity* _block);

	Entity* CreatePowerUp(PowerUpType _powerUp);
public:
	SpawnPowerUpSystem(World* _world);
	~SpawnPowerUpSystem();

	void Update(float _dt);
};



#endif