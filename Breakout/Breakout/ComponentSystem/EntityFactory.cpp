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
	b2BodyDef* bodyDef = new b2BodyDef();
	std::vector<b2FixtureDef*> fixDefs = std::vector<b2FixtureDef*>();
	b2PolygonShape* polyShapes;
	int polyCount;

	switch (_entityType)
	{
	case EntityFactory::BLOCK:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(200.0f);
		_entity->AddComponent<HealthComponent>(10);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		_entity->AddComponent<ScoreComponent>().m_score = 1;
		_entity->AddComponent<BlockComponent>();
		
		_entity->AddComponent<EffectComponent>().m_effects = EffectFlags::SHATTER | EffectFlags::EXPLORE;
		_entity->AddComponent<ShatterComponent>();
		break;
	case EntityFactory::PAD:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "pad";
		_entity->AddComponent<MouseInputComponent>();
		_entity->AddComponent<SpawnEntityComponent>(2, VECTOR3(0, 2, 0));
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Kettle-Drum-1.wav";
		_entity->AddComponent<EffectComponent>().m_effects = EffectFlags::INVISIBLE;
		break;
	case EntityFactory::BALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
		_entity->AddComponent<VelocityComponent>();
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<CollisionStatsComponent>(0.0f, 1000.0f, 40.0f, 20.0f);
		_entity->AddComponent<DamageComponent>(10);
		_entity->AddComponent<LoseLifeComponent>();
		break;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "wall";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(50.0f);
		break;
	case EntityFactory::H_WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "wallH";
		PhysicsSystem::GenerateBody(INVISIBLE_WALL, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		break;
	case EntityFactory::INVISIBLE_WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		break;
	case EntityFactory::PROJECTILE:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "projectile";
		_entity->AddComponent<VelocityComponent>();
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
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
        case EntityFactory::TEXT:
		_entity->AddComponent<TextComponent>();
		break;
	default:
		break;
	}
}

void EntityFactory::CreateBlockField(World* _world)
{
	/*	New Implementation	*/
	Entity* e;
	int xBlock, yBlock;
	int	startX, startY;

	startX = -20;
	xBlock = 2 * -startX;
	startY = 6;
	yBlock = 4;

	for (int y = startY; y < startY + yBlock; ++y)
		for (int x = startX; x < startX + xBlock; ++x)
		{
			e = _world->CreateEntity();
			EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
			e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(x, y, 0));
			e->GetComponent<RotationComponent>()->SetRotation(MacroRotateYawPitchRoll(0, 0, 0));
			BlockComponent* bc = e->GetComponent<BlockComponent>();


			_world->AddEntity(e);
		}




}