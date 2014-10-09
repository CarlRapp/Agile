#include "MovementSystem.h"


MovementSystem::MovementSystem(World* _world)
: Base(ComponentFilter().Requires<PositionComponent, VelocityComponent, CollisionComponent, MouseInputComponent>(), _world)
{
}

MovementSystem::~MovementSystem()
{

}


void MovementSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		auto mouse = e->GetComponent<MouseInputComponent>();
		auto position = e->GetComponent<PositionComponent>();
		auto velocity = e->GetComponent<VelocityComponent>();
		auto collision = e->GetComponent<CollisionComponent>();

		velocity->m_velocity = VECTOR3(mouse->m_controls.MouseDX, 0, 0);
	}
	
}