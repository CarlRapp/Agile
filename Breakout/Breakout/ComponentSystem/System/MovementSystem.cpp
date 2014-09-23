#include "MovementSystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"

MovementSystem::MovementSystem(World* _world)
: Base(ComponentFilter().Requires<VelocityComponent, PositionComponent>(), _world)
{
}

MovementSystem::~MovementSystem()
{

}


void MovementSystem::Update(float _dt)
{
	/*
	PositionComponent* position;
	VelocityComponent* velocity;
	EntityMap::iterator it;

	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		velocity = e->GetComponent<VelocityComponent>();
		if (ISZERO(velocity->m_velocity))
			continue;

		position = e->GetComponent<PositionComponent>()->SetPosition(;

		position->m_position.x += velocity->m_velocity.x * _dt;
		position->m_position.y += velocity->m_velocity.y * _dt;
		position->m_position.z += velocity->m_velocity.z * _dt;

		position->m_deltaPosition.x = velocity->m_velocity.x * _dt;
		position->m_deltaPosition.y = velocity->m_velocity.y * _dt;
		position->m_deltaPosition.z = velocity->m_velocity.z * _dt;
	}
	*/
}