#ifndef BLOCKSYSTEM_H
#define BLOCKSYSTEM_H

#include "ISystem.h"

class BlockSystem : public System<BlockSystem>
{
private:
	void OnEntityAdded(Entity* _e);
	void OnEntityRemove(Entity* _e);

public:
	BlockSystem(World* _world);
	~BlockSystem();

	void Update(float _dt);
};

#endif