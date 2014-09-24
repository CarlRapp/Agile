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
		{
			collision->GetBody()->SetLinearVelocity(b2Vec2(velocity->m_velocity.x, velocity->m_velocity.y));

			/*for (b2ContactEdge* contactEdge = collision->m_body->GetContactList(); contactEdge; contactEdge = contactEdge->next)
			{
				b2Contact* contact = contactEdge->contact;
				if (!contact->IsTouching())
					continue;
				
				b2WorldManifold worldManifold;
				contact->GetWorldManifold(&worldManifold);

				b2Fixture* fixture = contact->GetFixtureB();
				if (fixture->GetFilterData().categoryBits == CollisionCategory::BALL)
				{
					//fixture->GetBody()->ApplyForceToCenter(b2Vec2(0, 1000.0f), true);
					b2Vec2 distToCollision = (worldManifold.points[0] - fixture->GetBody()->GetPosition());
					float magn = 1 / sqrtf(distToCollision.x * distToCollision.x + distToCollision.y * distToCollision.y); 
					distToCollision = b2Vec2(distToCollision.x * magn, distToCollision.y * magn);
					if (distToCollision.y < 0)
					fixture->GetBody()->SetLinearVelocity(b2Vec2(-distToCollision.x * 10.0f, -distToCollision.y * 10.0f));
				}
			}*/
		}
	}
	
}