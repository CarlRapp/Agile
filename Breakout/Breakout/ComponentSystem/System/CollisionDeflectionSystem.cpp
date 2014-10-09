#include "CollisionDeflectionSystem.h"

#include "../World.h"

CollisionDeflectionSystem::CollisionDeflectionSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>(), _world)
{

}

CollisionDeflectionSystem::~CollisionDeflectionSystem()
{

}

void CollisionDeflectionSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		b2Body* b2Body = collision->GetBody();

		// Update bounce
		std::vector<CollisionContact> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			Entity* otherEntity = m_world->GetEntity(collisions[i].m_otherId);
			if (otherEntity->GetState() != Entity::ALIVE)
				continue;
			auto otherDeflection = otherEntity->GetComponent<DeflectionComponent>();
			if (otherDeflection)
			{
				float speed = b2Body->GetLinearVelocity().Length();
				if (speed < otherDeflection->GetDeflection() && speed != 0)
				{
					b2Vec2 velocity = b2Body->GetLinearVelocity();
					float deflection = otherDeflection->GetDeflection();
					e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3
						(
						(velocity.x / speed) * deflection,
						(velocity.y / speed) * deflection,
						e->GetComponent<VelocityComponent>()->m_velocity.z
						);
					
				}
			}
		}
	}
}