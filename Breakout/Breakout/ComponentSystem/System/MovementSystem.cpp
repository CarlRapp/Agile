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

		velocity->m_velocity = VECTOR3(mouse->m_controls.MouseDX * 5.f, -mouse->m_controls.MouseDY * 5.f, 0);

		if (collision->IsAdded())
			collision->GetBody()->SetLinearVelocity(b2Vec2(velocity->m_velocity.x, velocity->m_velocity.y));
	}
	
}