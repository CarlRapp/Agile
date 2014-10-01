#include "BlockSystem.h"
#include "../Component/BlockComponent.h"

BlockSystem::BlockSystem(World* _world)
: Base(ComponentFilter().Requires<BlockComponent>(), _world){}

BlockSystem::~BlockSystem()
{
}

void BlockSystem::Update(float _dt)
{


}




void BlockSystem::OnEntityAdded(Entity* _e)
{
	printf("Block %d added!\n", _e->GetId());
}


void BlockSystem::OnEntityRemove(Entity* _e)
{
	printf("Block %d Removed!\n", _e->GetId());
}
