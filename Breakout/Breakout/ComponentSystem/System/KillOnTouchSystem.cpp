#include "KillOnTouchSystem.h"
#include "../Component/CollisionComponent.h"
#include "PhysicsSystem.h"
#include "../World.h"

KillOnTouchSystem::KillOnTouchSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>(), _world)
{
}

KillOnTouchSystem::~KillOnTouchSystem()
{

}


void KillOnTouchSystem::Update(float _dt)
{
	std::vector<Entity*> toBeRemoved;
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		if ((e->GetState() != Entity::ALIVE))
			continue;

		auto damage = e->GetComponent<DamageComponent>();
		auto collision = e->GetComponent<CollisionComponent>();
		std::vector<CollisionContact> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			CollisionContact contact = collisions.at(i);
			Entity* collidingEntity = m_world->GetEntity(collisions[i].m_otherId);
			int bitA = contact.m_fixture->GetFilterData().categoryBits;
			int bitB = contact.m_otherFixture->GetFilterData().categoryBits;

			if (bitA == CollisionCategory::KILLONTOUCH)
			{
				toBeRemoved.push_back(collidingEntity);
				break;
			}
			else if (bitB == CollisionCategory::KILLONTOUCH)
			{
				toBeRemoved.push_back(e);
				break;
			}
			

		}
	}
	for (auto entity : toBeRemoved)
		entity->SetState(Entity::SOON_DEAD);
}
