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
		_entity->AddComponent<CollisionComponent>(PhysicsSystem::GenerateFixtureDefinition(_entityType));
		_entity->AddComponent<HealthComponent>(10);
		_entity->AddComponent<DeflectionComponent>(1.0f);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		_entity->AddComponent<ScoreComponent>().m_score = 1;
		break;
	case EntityFactory::PAD:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "pad";
		_entity->AddComponent<MouseInputComponent>();
		_entity->AddComponent<CollisionComponent>(PhysicsSystem::GenerateFixtureDefinition(_entityType));
		_entity->AddComponent<AudioComponent>().m_audioPath = "Kettle-Drum-1.wav";
		_entity->AddComponent<DeflectionComponent>(1.0f);
		break;
	case EntityFactory::BALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
		_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<CollisionComponent>(PhysicsSystem::GenerateFixtureDefinition(_entityType));
		_entity->AddComponent<DamageComponent>(10);
		_entity->AddComponent<BounceComponent>(1.0f);
		_entity->AddComponent<PlayerLifeComponent>();
		break;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "wall";
		_entity->AddComponent<CollisionComponent>(PhysicsSystem::GenerateFixtureDefinition(_entityType));
		break;
	case EntityFactory::H_WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "wallH";
		_entity->AddComponent<CollisionComponent>(PhysicsSystem::GenerateFixtureDefinition(INVISIBLE_WALL));
		break;
	case EntityFactory::INVISIBLE_WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<CollisionComponent>(PhysicsSystem::GenerateFixtureDefinition(_entityType));
		break;
	case EntityFactory::PROJECTILE:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "projectile";
		_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<CollisionComponent>(PhysicsSystem::GenerateFixtureDefinition(_entityType));
		break;
	case EntityFactory::PLAYER:
		_entity->AddComponent<PlayerComponent>();
		_entity->AddComponent<LifeComponent>();
		_entity->AddComponent<ScoreComponent>().m_score = 0;
		break;
	case EntityFactory::POINTLIGHT:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<LightComponent>();
		break;
	case EntityFactory::PLANE:
		_entity->AddComponent<PositionComponent>().SetPosition(VECTOR3(0, 0, 0));
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "coolPlane";
		break;
	default:
		break;
	}

	if (_entity)
		_entity->SetState(Entity::ALIVE);
}