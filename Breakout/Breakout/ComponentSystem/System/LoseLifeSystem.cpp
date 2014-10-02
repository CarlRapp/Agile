#include "LoseLifeSystem.h"
#include "../World.h"

LoseLifeSystem::LoseLifeSystem(World* _world)
: Base(ComponentFilter().Requires<LoseLifeComponent>(), _world)
{
}

LoseLifeSystem::~LoseLifeSystem()
{

}

void LoseLifeSystem::OnEntityAdded(Entity* _e)
{
	printf("Life Added!\n");
}

void LoseLifeSystem::OnEntityRemoved(Entity* _e)
{
	printf("Life Removed!\n");

	Entity* player = m_world->GetEntities<LifeComponent>()->at(0);
	player->GetComponent<LifeComponent>()->m_noLifes -= _e->GetComponent<LoseLifeComponent>()->m_loseLife;
	if (player->GetComponent<PlayerComponent>()->m_playerLife <= 0)
		m_world->Kill();
}