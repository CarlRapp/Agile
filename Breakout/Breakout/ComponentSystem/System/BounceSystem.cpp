#include "BounceSystem.h"

#include "../World.h"

BounceSystem::BounceSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>().RequiresOneOf<BounceComponent>(), _world)
{

}

BounceSystem::~BounceSystem()
{

}

void BounceSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		if (e->GetState() == Entity::DEAD)
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		auto bounce = e->GetComponent<BounceComponent>();
		b2Body* b2Body = collision->GetBody();
		std::vector<int> collisions = collision->GetCollisions();
		/*
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			Entity* collidingEntity = m_world->GetEntity(collisions[i]);
			auto collidingDeflection = collidingEntity->GetComponent<DeflectionComponent>();
			if (collidingDeflection)
			{
				auto collidingCollision = collidingEntity->GetComponent<CollisionComponent>();
				const b2WorldManifold* manifold = collision->GetManifold(collidingEntity->GetId());
				b2Vec2 velocity = b2Body->GetLinearVelocity();
				b2Body->SetLinearVelocity(b2Vec2(velocity.x * manifold->normal.x, velocity.y * manifold->normal.y));
				printf("Normal x:%f, y:%f\n", manifold->normal.x, manifold->normal.y);
				printf("Before vx:%f, vy:%f\n", velocity.x, velocity.y);
				printf("After vx:%f, vy:%f\n", b2Body->GetLinearVelocity().x, b2Body->GetLinearVelocity().y);
			}
		}*/
	}
}