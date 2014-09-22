#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H


#include "ISystem.h"

class ProjectileSystem : public System<ProjectileSystem>
{
private:

	std::vector<Entity*> m_entites;


public:

	ProjectileSystem(World* _world);
	~ProjectileSystem();

	void Update(float _dt);
};


#endif
