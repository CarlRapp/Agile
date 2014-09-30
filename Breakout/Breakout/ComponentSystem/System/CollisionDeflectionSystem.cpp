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
		if (e->GetState() == Entity::DEAD)
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		b2Body* b2Body = collision->GetBody();

		// Update bounce
		std::vector<int> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			Entity* otherEntity = m_world->GetEntity(collisions[i]);
			if (otherEntity->GetState() == Entity::DEAD)
				continue;
			auto otherDeflection = otherEntity->GetComponent<DeflectionComponent>();
			if (otherDeflection)
			{
				float speed = b2Body->GetLinearVelocity().Length();
				if (speed < otherDeflection->GetDeflection())
				{
					b2Body->SetLinearVelocity(b2Vec2
					(
					(b2Body->GetLinearVelocity().x / speed) * otherDeflection->GetDeflection(),
					(b2Body->GetLinearVelocity().y / speed) * otherDeflection->GetDeflection()
					));
				}

				/*if (b2Body->GetLinearVelocity().x >= 0)
					velocityX = b2Body->GetLinearVelocity().x + otherDeflection->GetDeflection();
				else
					velocityX = b2Body->GetLinearVelocity().x - otherDeflection->GetDeflection();
				if (b2Body->GetLinearVelocity().y >= 0)
					velocityY = b2Body->GetLinearVelocity().y + otherDeflection->GetDeflection();
				else
					velocityY = b2Body->GetLinearVelocity().y - otherDeflection->GetDeflection();
				*/
				

				//b2Body->SetLinearVelocity(b2Vec2(b2Body->GetLinearVelocity().x * manifold.normal.x, b2Body->GetLinearVelocity().y * -manifold.normal.y));

				// STUDSA MANUELLT

				/*

				b2Body->SetLinearVelocity(b2Vec2(velocityX, velocityY));
				*/
			}
		}
	}
}