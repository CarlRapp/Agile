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
	_e->SetState(Entity::SOON_DEAD);
	Entity* player = m_world->GetEntities<LifeComponent>()->at(0);

	player->GetComponent<LifeComponent>()->m_noLifes -= 1;
	player->GetComponent<LifeComponent>()->SetString();
	if (player->GetComponent<LifeComponent>()->m_noLifes <= 0)
		m_world->Kill();
}

void LoseLifeSystem::OnEntityRemoved(Entity* _e)
{

}