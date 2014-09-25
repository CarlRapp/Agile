#include "KillObjectSystem.h"
#include "../World.h"

KillObjectSystem::KillObjectSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>().RequiresOneOf<HealthComponent, DamageComponent>(), _world)
{
}

KillObjectSystem::~KillObjectSystem()
{

}


void KillObjectSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		auto collision = e->GetComponent<CollisionComponent>();
		auto health = e->GetComponent<HealthComponent>();
		auto damage = e->GetComponent<DamageComponent>();
		b2Body* body = collision->GetBody();

		std::vector<int> collisions = collision->GetCollisions();
		for (int i = 0; i < collisions.size(); ++i)
		{
		}
	}
}