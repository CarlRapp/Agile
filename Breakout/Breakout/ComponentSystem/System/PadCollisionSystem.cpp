#include "PadCollisionSystem.h"

PadCollisionSystem::PadCollisionSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>(), _world)
{
}

PadCollisionSystem::~PadCollisionSystem()
{

}

void PadCollisionSystem::Update(float _dt)
{

}