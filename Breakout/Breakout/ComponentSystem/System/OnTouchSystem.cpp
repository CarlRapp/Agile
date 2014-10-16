#include "OnTouchSystem.h"
#include "../Component/CollisionComponent.h"
#include "PhysicsSystem.h"
#include "../World.h"

OnTouchSystem::OnTouchSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>(), _world)
{
}

OnTouchSystem::~OnTouchSystem()
{

}


void OnTouchSystem::Update(float _dt)
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

			if (bitA & CollisionCategory::KILLONTOUCH)
			{
				toBeRemoved.push_back(collidingEntity);
				break;
			}
			if (bitA & CollisionCategory::DIEONTOUCH)
			{
				toBeRemoved.push_back(e);
			}
		}
	}
	for (auto entity : toBeRemoved)
		entity->SetState(Entity::SOON_DEAD);
}
