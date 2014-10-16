#include "RespawnBallSystem.h"
#include "../World.h"
#include "../Component/BallComponent.h"

RespawnBallSystem::RespawnBallSystem(World* _world)
: Base(ComponentFilter().Requires<BallComponent>(), _world), m_numBallsLeft(0)
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
					e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(rand() % 20 - 10, 40, 0);
					e->GetComponent<ScaleComponent>()->SetScale(VECTOR3(0.8f, 0.8f, 0.8f));
					e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(position->GetPosition().x + spawnEntity->m_posFromEntity.x, position->GetPosition().y + spawnEntity->m_posFromEntity.y, position->GetPosition().z + spawnEntity->m_posFromEntity.z));
					e->GetComponent<EffectComponent>()->m_effects.OnAdded = EffectFlags::TRAIL;
					m_world->AddEntity(e);
				}
			}
		}

	//	kill balls outside range
	std::vector<Entity*>* balls = m_world->GetEntities<BallComponent>();
	if (balls)
	{
		for (int i = balls->size() - 1; i >= 0; --i)
		{
			PositionComponent* POS = balls->at(i)->GetComponent<PositionComponent>();
			if (!POS)
			{
				balls->at(i)->SetState(Entity::SOON_DEAD);
				continue;
			}
			Entity* tBall = balls->at(i);
			VECTOR3 vPos = POS->GetPosition();
			if (vPos.x < -100 || vPos.x > 100)
				tBall->SetState(Entity::SOON_DEAD);
			if (vPos.y < -100 || vPos.y > 100)
				tBall->SetState(Entity::SOON_DEAD);

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
	if (m_numBallsLeft == 0)
	{
		Entity* e = m_world->CreateEntity();
		e->AddComponent<LoseLifeComponent>();
		m_world->AddEntity(e);
	}
}