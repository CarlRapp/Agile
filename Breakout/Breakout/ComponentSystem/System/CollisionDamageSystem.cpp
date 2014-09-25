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
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		auto damage = e->GetComponent<DamageComponent>();
		if ((e->GetState() == Entity::DEAD) || !damage)
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		std::vector<int> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			Entity* collidingEntity = m_world->GetEntity(collisions[i]);
			auto collidingHealth = collidingEntity->GetComponent<HealthComponent>();
			if (collidingHealth)
			{
				DealDamage(*damage, *collidingHealth);
				if (collidingHealth->m_currentHealth <= 0)
				{
					collidingEntity->SetState(Entity::DEAD);
					break;
				}
			}
		}
	}
}

void CollisionDamageSystem::DealDamage(DamageComponent& _damage, HealthComponent& _health) const
{
	_health.m_currentHealth -= _damage.m_damage;
}