#include "MovementSystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"
#include "../World.h"

MovementSystem::MovementSystem(World* _world)
: Base(ComponentFilter().Requires<VelocityComponent, PositionComponent>(), _world)
{
}

MovementSystem::~MovementSystem()
{

}


void MovementSystem::Update(float _dt)
{

	VelocityComponent* velocity;
	PositionComponent* position;


	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		velocity = m_entities[i]->GetComponent<VelocityComponent>();
		position = m_entities[i]->GetComponent<PositionComponent>();

		if (velocity->m_velocity.x != 0)
		{

			position->m_deltaPosition.x = velocity->m_velocity.x * _dt * 2.f;
			
			float x = position->m_position.x + position->m_deltaPosition.x;
			x = x > 13.5f ? x = 13.5f : x = x;
			x = x < 0 ? x = 0 : x = x;
			position->m_position.x = x;

		}

	}

}