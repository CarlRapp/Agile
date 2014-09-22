#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() 
: Base(ComponentFilter().Requires<VelocityComponent, PositionComponent, CollisionComponent>())
{
	m_b2World = new b2World(DEFAULTGRAVITY);
}


PhysicsSystem::~PhysicsSystem()
{
	delete m_b2World;
}

void PhysicsSystem::Update(float _dt)
{
	m_b2World->Step(_dt, VELOCITYITERATIONS, POSITIONITERATIONS);
	if (m_b2World->GetBodyList() != 0)
	{
		b2Vec2 position = m_b2World->GetBodyList()->GetPosition();
		float32 angle = m_b2World->GetBodyList()->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}

}