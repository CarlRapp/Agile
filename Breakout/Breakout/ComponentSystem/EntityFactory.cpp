#include "../stdafx.h"

#include "EntityFactory.h"


EntityFactory* EntityFactory::m_entityFactory = 0;

EntityFactory* EntityFactory::GetInstance()
{
	if (m_entityFactory)
		return m_entityFactory;

	m_entityFactory = new EntityFactory();

	return m_entityFactory;
}


void EntityFactory::CreateEntity(Entity* _entity, EntityType _entityType)
{
	switch (_entityType)
	{
	case EntityFactory::BLOCK:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "box";
		_entity->AddComponent<ScoreComponent>();
		_entity->AddComponent<HealthComponent>();
		break;
	case EntityFactory::PAD:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "box";
		_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<InputComponent>();
		break;
	case EntityFactory::BALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
		_entity->AddComponent<VelocityComponent>();
		break;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::PLAYER:
		_entity->AddComponent<ScoreComponent>().m_score = 0;
		_entity->AddComponent<LifeComponent>();
	default:
		break;
	}
}
