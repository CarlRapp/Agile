#include "CollisionDamageSystem.h"
#include "../World.h"

CollisionDamageSystem::CollisionDamageSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent, DamageComponent>(), _world)
{
}

CollisionDamageSystem::~CollisionDamageSystem()
{

}


void CollisionDamageSystem::Update(float _dt)
{
	vector<Entity*> toBeRemoved;
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
			Entity* collidingEntity = m_world->GetEntity(collisions[i].m_otherId);
			auto collidingHealth = collidingEntity->GetComponent<HealthComponent>();
			if (collidingHealth)
			{

				if (collidingHealth->m_currentHealth == -1337)
					break;

				DealDamage(*damage, *collidingHealth);
				if (collidingHealth->m_currentHealth <= 0)
				{
					toBeRemoved.push_back(collidingEntity);
					break;
				}
			}
		}
	}
	for (auto entity : toBeRemoved)
		entity->SetState(Entity::SOON_DEAD);
}

void CollisionDamageSystem::DealDamage(DamageComponent& _damage, HealthComponent& _health) const
{
	_health.m_currentHealth -= _damage.m_damage;
}