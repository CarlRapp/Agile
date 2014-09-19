#include "MovementSystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"

MovementSystem::MovementSystem()
: Base(ComponentFilter().Requires<VelocityComponent, PositionComponent>())
{
}

MovementSystem::~MovementSystem()
{

}


void MovementSystem::Update(float _dt)
{
	/*if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
		continue;*/
}