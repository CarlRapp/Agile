#include "ScoreSystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/ScoreComponent.h"
#include "../World.h"
#include "../EntityFactory.h"

ScoreSystem::ScoreSystem(World* _world)
: Base(ComponentFilter().Requires<ScoreComponent, CollisionComponent>(), _world)
{
}

ScoreSystem::~ScoreSystem()
{

}

void ScoreSystem::OnEntityAdded(Entity* _e)
{

}
void ScoreSystem::OnEntityRemoved(Entity* _e)
{

}
void ScoreSystem::Update(float _dt)
{
	std::vector<Entity*>* player = m_world->GetEntities<PlayerComponent>();

	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		if ((e->GetState() != Entity::ALIVE))
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		std::vector<CollisionContact> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			//Entity* collidingEntity = m_world->GetEntity(collisions[i]);
			auto SC = player->at(0)->GetComponent<ScoreComponent>();
                        SC->m_score += e->GetComponent<ScoreComponent>()->m_score;
                        SC->SetString();
                        printf("%d\n",SC->m_score);
                        
			//printf("Score: %i\n", player->at(0)->GetComponent<ScoreComponent>()->m_score);
		}
	}

}