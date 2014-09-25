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


void ScoreSystem::Update(float _dt)
{
	std::vector<Entity*>* player = m_world->GetEntities<PlayerComponent>();


	EntityMap::iterator it;
	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;


		auto collision = e->GetComponent<CollisionComponent>();


		if (collision->IsAdded())
		{
			for (b2ContactEdge* contactEdge = collision->GetBody()->GetContactList(); contactEdge; contactEdge = contactEdge->next)
			{
				b2Contact* contact = contactEdge->contact;
				if (!contact->IsTouching())
				{
					player->at(0)->GetComponent<ScoreComponent>()->m_score += e->GetComponent<ScoreComponent>()->m_score;
					printf("Score: %i\n", player->at(0)->GetComponent<ScoreComponent>()->m_score);
				}
			}
		}


	}


}