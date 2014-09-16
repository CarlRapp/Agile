#include "../stdafx.h"

#include "EntityFactory.h"

#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/ModelComponent.h"

EntityFactory* EntityFactory::m_entityFactory = 0;
bool EntityFactory::m_initialized = false;

EntityFactory* EntityFactory::GetInstance()
{
	if (m_entityFactory)
		return m_entityFactory;

	m_entityFactory = new EntityFactory();

	return m_entityFactory;
}

void EntityFactory::Initialize(Entity** _entites)
{
	m_entites = _entites;
	m_initialized = true;
}


Entity* EntityFactory::CreateEntity(EntityType _entityType)
{
	if (!m_initialized)
	{
		printf("EntityFactory not initialized!\n");
		return 0;
	}


	Entity* entity = 0;

	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		if (m_entites[i]->GetState() == Entity::DEAD)
		{
			entity = m_entites[i];
			break;
		}
	}

	if (entity == 0)
	{
		printf("Max number of entites already in use!\n");
		return 0;
	}

	switch (_entityType)
	{
	case EntityFactory::BLOCK:
		entity->AddComponent<PositionComponent>();
		entity->AddComponent<RotationComponent>();
		entity->AddComponent<ScaleComponent>();
		entity->AddComponent<ModelComponent>();
		entity->AddComponent<ScoreComponent>();
		entity->AddComponent<HealthComponent>();
		break;
	case EntityFactory::PAD:
		entity->AddComponent<PositionComponent>();
		entity->AddComponent<RotationComponent>();
		entity->AddComponent<ScaleComponent>();
		entity->AddComponent<ModelComponent>();
		entity->AddComponent<VelocityComponent>();
		break;
	case EntityFactory::BALL:
		entity->AddComponent<PositionComponent>();
		entity->AddComponent<RotationComponent>();
		entity->AddComponent<ScaleComponent>();
		entity->AddComponent<ModelComponent>();
		entity->AddComponent<VelocityComponent>();
		break;
	case EntityFactory::POWERUP:
		break;
	default:
		break;
	}

	return entity;
}

void EntityFactory::DeleteEntity(Entity* _entity)
{
	_entity->RemoveAllComponents();
	_entity->SetState(Entity::DEAD);
}