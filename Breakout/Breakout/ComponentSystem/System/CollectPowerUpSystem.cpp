#include "CollectPowerUpSystem.h"
#include "../Component/CollisionComponent.h"
#include "PhysicsSystem.h"
#include "../World.h"

CollectPowerUpSystem::CollectPowerUpSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>(), _world)
{
}

CollectPowerUpSystem::~CollectPowerUpSystem()
{

}

void CollectPowerUpSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		if ((e->GetState() != Entity::ALIVE))
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		std::vector<CollisionContact> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			CollisionContact contact = collisions.at(i);
			int bitA = contact.m_fixture->GetFilterData().categoryBits;
			int bitB = contact.m_otherFixture->GetFilterData().categoryBits;
			
			if ((bitA == CollisionCategory::POWERUP && bitB == CollisionCategory::PAD) || (bitA == CollisionCategory::PAD && bitB == CollisionCategory::POWERUP))
			{
				int a = 2;
			}

		}
	}
}
