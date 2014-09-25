#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H


#include "ISystem.h"

class ProjectileSystem : public System<ProjectileSystem>
{
private:

public:

	ProjectileSystem(World* _world);
	~ProjectileSystem();

	void Update(float _dt);
};


#endif
