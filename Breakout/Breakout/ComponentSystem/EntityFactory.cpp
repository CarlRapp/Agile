#include "../stdafx.h"

#include "EntityFactory.h"

#include "Component/PositionComponent.h"
#include "Component/RotationComponent.h"
#include "Component/ScaleComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/HealthComponent.h"
#include "Component/ScoreComponent.h"
#include "Component/ModelComponent.h"
#include "Component/LifeComponent.h"

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
		entity->AddComponent<ModelComponent>().m_modelPath = "block";
		entity->AddComponent<ScoreComponent>();
		entity->AddComponent<HealthComponent>();
		break;
	case EntityFactory::PAD:
		entity->AddComponent<PositionComponent>();
		entity->AddComponent<RotationComponent>();
		entity->AddComponent<ScaleComponent>();
		entity->AddComponent<ModelComponent>().m_modelPath = "pad";
		entity->AddComponent<VelocityComponent>();
		break;
	case EntityFactory::BALL:
		entity->AddComponent<PositionComponent>();
		entity->AddComponent<RotationComponent>();
		entity->AddComponent<ScaleComponent>();
		entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
		entity->AddComponent<VelocityComponent>();
		break;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::PLAYER:
		entity->AddComponent<ScoreComponent>().m_score = 0;
		entity->AddComponent<LifeComponent>();
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