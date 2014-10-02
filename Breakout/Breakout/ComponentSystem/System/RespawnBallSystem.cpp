#include "RespawnBallSystem.h"
#include "../World.h"

RespawnBallSystem::RespawnBallSystem(World* _world)
: Base(ComponentFilter().RequiresOneOf<LoseLifeComponent, SpawnEntityComponent>(), _world), m_numBallsLeft(0)
{
}

RespawnBallSystem::~RespawnBallSystem()
{

}


void RespawnBallSystem::Update(float _dt)
{
	// Spawn ball for pad
	if (m_numBallsLeft == 0 && m_world->IsAlive())
		for (auto entityPair : m_entityMap)
		{
			Entity* e = entityPair.second;
			if (e->GetState() == Entity::DEAD)
				continue;
			auto spawnEntity = e->GetComponent<SpawnEntityComponent>();
			auto mouse = e->GetComponent<MouseInputComponent>();
			auto position = e->GetComponent<PositionComponent>();
			if (mouse && position && spawnEntity->m_entityType == EntityFactory::BALL)
			{
				if (mouse->m_controls.LeftButton == InputState::Pressed)
				{
					Entity* e = m_world->CreateEntity();
					EntityFactory::GetInstance()->CreateEntity(e, (EntityFactory::EntityType)spawnEntity->m_entityType);
					e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(0, 40, 0);
					e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(position->GetPosition().x + spawnEntity->m_posFromEntity.x, position->GetPosition().y + spawnEntity->m_posFromEntity.y, position->GetPosition().z + spawnEntity->m_posFromEntity.z));

					m_world->AddEntity(e);
				}
			}
		}
}

void RespawnBallSystem::OnEntityAdded(Entity* _e)
{
	auto loseLife = _e->GetComponent<LoseLifeComponent>();
	if (loseLife)
		++m_numBallsLeft;
}

void RespawnBallSystem::OnEntityRemoved(Entity* _e)
{
	auto loseLife = _e->GetComponent<LoseLifeComponent>();
	if (loseLife)
		--m_numBallsLeft;
}