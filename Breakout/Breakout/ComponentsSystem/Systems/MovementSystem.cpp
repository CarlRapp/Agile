#include "MovementSystem.h"
#include "../Components/VelocityComponent.h"
#include "../Components/PositionComponent.h"

MovementSystem::MovementSystem()
: Base(ComponentFilter().Requires<VelocityComponent, PositionComponent>())
{
}

MovementSystem::~MovementSystem()
{

}


void MovementSystem::Update(double _dt)
{
	PositionComponent* position;
	VelocityComponent* velocity;

	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;


		position = m_entities[i]->GetComponent<PositionComponent>();
		velocity = m_entities[i]->GetComponent<VelocityComponent>();

		Vector3 hest = velocity->m_velocity * _dt;
		velocity->m_velocity += velocity->m_velocity;

		position->m_position += velocity->m_velocity * _dt;


		//m_entities[i]->SetState(Entity::LIMBO);

	}
}