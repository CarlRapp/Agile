#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() : Base(ComponentFilter().Requires<VelocityComponent, PositionComponent>().RequiresOneOf<BoxComponent, SphereComponent>())
{
	m_world = new b2World(b2Vec2(0,0));
}

PhysicsSystem::PhysicsSystem(const b2Vec2& _gravity) : Base(ComponentFilter().Requires<VelocityComponent, PositionComponent>().RequiresOneOf<BoxComponent, SphereComponent>())
{
	m_world = new b2World(_gravity);
}

PhysicsSystem::~PhysicsSystem()
{
	delete m_world;
}

void PhysicsSystem::Update(double _dt)
{
	//for (b2Body* body = m_world->GetBodyList(); body; body->GetNext())
	{
		
	}
	m_world->Step(_dt, VELOCITYITERATIONS, POSITIONITERATIONS);
	b2Vec2 position = m_world->GetBodyList()->GetPosition();
	float32 angle = m_world->GetBodyList()->GetAngle();
	printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}