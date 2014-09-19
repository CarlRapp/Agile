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
	PositionComponent* position;
	VelocityComponent* velocity;

	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		
		velocity = m_entities[i]->GetComponent<VelocityComponent>();
		if (ISZERO(velocity->m_velocity))
			continue;

		position = m_entities[i]->GetComponent<PositionComponent>();

		position->m_position.x += velocity->m_velocity.x * _dt;
		position->m_position.y += velocity->m_velocity.y * _dt;
		position->m_position.z += velocity->m_velocity.z * _dt;

		position->m_deltaPosition.x = velocity->m_velocity.x * _dt;
		position->m_deltaPosition.y = velocity->m_velocity.y * _dt;
		position->m_deltaPosition.z = velocity->m_velocity.z * _dt;
	}
}