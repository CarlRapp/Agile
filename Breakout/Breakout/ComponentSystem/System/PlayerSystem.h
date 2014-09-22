#ifndef PLAYERSYSTEM_H
#define PLAYERSYSTEM_H

#include "ISystem.h"

class PlayerSystem : public System<PlayerSystem>
{
private:
public:

	PlayerSystem(World* _world);
	~PlayerSystem();

	void Update(float _dt);
};
#endif