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
		_entity->AddComponent<CollisionComponent>();
		_entity->AddComponent<VelocityComponent>();
		break;
	case EntityFactory::PAD:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "box";
		_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<CollisionComponent>();
		_entity->AddComponent<MouseInputComponent>();
		break;
	case EntityFactory::BALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
		_entity->AddComponent<CollisionComponent>();
		_entity->AddComponent<VelocityComponent>();
		break;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<CollisionComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "wall";
	default:
		break;
	}
}