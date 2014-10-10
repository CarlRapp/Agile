#include "ScoreSystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/ScoreComponent.h"
#include "../World.h"
#include "../EntityFactory.h"

ScoreSystem::ScoreSystem(World* _world)
: Base(ComponentFilter().Requires<ScoreComponent>(), _world)
{
    m_frameScore = 0;
}

ScoreSystem::~ScoreSystem()
{

}

void ScoreSystem::OnEntityAdded(Entity* _e)
{

}

void ScoreSystem::OnEntityRemoved(Entity* _e)
{
	ScoreComponent* SC = _e->GetComponent<ScoreComponent>();
	int a = 2;

	m_frameScore += SC->m_score;
}
void ScoreSystem::Update(float _dt)
{
	std::vector<Entity*>* player = m_world->GetEntities<PlayerComponent>();
	if (m_frameScore > 0)
	{
		auto SC = player->at(0)->GetComponent<ScoreComponent>();
		SC->AddScore(m_frameScore);
		SC->SetString();
		m_frameScore = 0;
	}
}