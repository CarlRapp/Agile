#include "MovementSystem.h"
#include "../Components/VelocityComponent.h"
#include "../Components/PositionComponent.h"

MovementSystem::MovementSystem()
: Base(ComponentFilter().Requires<VelocityComponent, PositionComponent>())
{
}


void MovementSystem::Update(double _dt)
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		float x = m_entities[i]->GetComponent<PositionComponent>()->m_X += _dt;
		float y = m_entities[i]->GetComponent<PositionComponent>()->m_Y += _dt;
		float z = m_entities[i]->GetComponent<PositionComponent>()->m_Z += _dt;

		printf("Entity:%i was moved to new position: %f, %f, %f\n", i, x, y, z);
	}
}