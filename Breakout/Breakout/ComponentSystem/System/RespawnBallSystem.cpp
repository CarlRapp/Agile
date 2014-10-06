#include "RespawnBallSystem.h"
#include "../World.h"

RespawnBallSystem::RespawnBallSystem(World* _world)
: Base(ComponentFilter().Requires<LoseLifeComponent>(), _world), m_numBallsLeft(0)
{
}

RespawnBallSystem::~RespawnBallSystem()
{
}


void RespawnBallSystem::Update(float _dt)
{
	// Spawn ball for pad
	if (m_numBallsLeft == 0 && m_world->IsAlive())
	for (auto entity = m_world->GetEntities<SpawnEntityComponent>()->begin(); entity != m_world->GetEntities<SpawnEntityComponent>()->end(); ++entity)
		{
			Entity* e = *entity;
			if (e->GetState() != Entity::ALIVE)
				continue;
			auto spawnEntity = e->GetComponent<SpawnEntityComponent>();
			auto mouse = e->GetComponent<MouseInputComponent>();
			auto position = e->GetComponent<PositionComponent>();
			if (spawnEntity->m_entityType == EntityFactory::BALL && mouse && position)
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
	++m_numBallsLeft;
}

void RespawnBallSystem::OnEntityRemoved(Entity* _e)
{
	--m_numBallsLeft;
}