#include "PlayerLifeSystem.h"
#include "../Component/PositionComponent.h"
#include "../World.h"

PlayerLifeSystem::PlayerLifeSystem(World* _world)
: Base(ComponentFilter().Requires<PlayerLifeComponent>(), _world)
{
}

PlayerLifeSystem::~PlayerLifeSystem()
{

}


void PlayerLifeSystem::Update(float _dt)
{
	EntityMap::iterator it;

	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() == Entity::DEAD)
			continue;

		if (e->GetComponent<PositionComponent>()->GetPosition().y < -20)
			e->SetState(Entity::DEAD);
	}

}

void PlayerLifeSystem::OnEntityAdded(Entity* _e)
{
	printf("Life Added!\n");
}
void PlayerLifeSystem::OnEntityRemove(Entity* _e)
{
	printf("Life Removed!\n");

	if (GetEntities().size() == 0)
		m_world->Kill();
}