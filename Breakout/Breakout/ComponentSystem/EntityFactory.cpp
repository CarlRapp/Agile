#include "../stdafx.h"

#include "EntityFactory.h"
#include "Component/BallComponent.h"


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
	case EntityFactory::STANDARD_BLOCK_RED:
	{
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1_red";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(30.0f);
		_entity->AddComponent<HealthComponent>(10);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		_entity->AddComponent<ScoreComponent>().m_score = 1;
		_entity->AddComponent<BlockComponent>();
		_entity->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		
		auto effect = &_entity->AddComponent<EffectComponent>();
		effect->m_effects.OnAdded = EffectFlags::SCALE_MIN_TO_MAX;
		break;
	}
	case EntityFactory::STANDARD_BLOCK_GREEN:
	{
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1_green";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(30.0f);
		_entity->AddComponent<HealthComponent>(10);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		_entity->AddComponent<ScoreComponent>().m_score = 1;
		_entity->AddComponent<BlockComponent>();
		_entity->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));

		auto effect = &_entity->AddComponent<EffectComponent>();
		effect->m_effects.OnAdded = EffectFlags::SCALE_MIN_TO_MAX;
		break;
	}
	case EntityFactory::STANDARD_BLOCK_BLUE:
	{
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
		_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1_blue";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(30.f);
		_entity->AddComponent<HealthComponent>(10);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		_entity->AddComponent<ScoreComponent>().m_score = 1;
		_entity->AddComponent<BlockComponent>();
		_entity->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		_entity->GetComponent<BlockComponent>()->SetDimension(VECTOR2(1, 1));

		auto effect = &_entity->AddComponent<EffectComponent>();
		effect->m_effects.OnAdded = EffectFlags::SCALE_MIN_TO_MAX;
		break;
	}
	case EntityFactory::INDESTRUCTIBLE_BLOCK:
	{
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1_black";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(30.f);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		_entity->AddComponent<ScoreComponent>().m_score = 1;
		_entity->AddComponent<BlockComponent>();
		_entity->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));

		auto effect = &_entity->AddComponent<EffectComponent>();
		effect->m_effects.OnAdded = EffectFlags::SCALE_MIN_TO_MAX;
		break;
	}
	case EntityFactory::TNT_BLOCK:
	{
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1_tnt";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(30.f);
		_entity->AddComponent<HealthComponent>(10);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Explosion.wav";
		_entity->AddComponent<ScoreComponent>().m_score = 1;
		_entity->AddComponent<BlockComponent>();
		_entity->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		_entity->AddComponent<TNTComponent>();

		auto effect = &_entity->AddComponent<EffectComponent>();
		effect->m_effects.OnAdded = EffectFlags::SCALE_MIN_TO_MAX;
		effect->m_effects.OnRemoved = EffectFlags::EXPLODE;
		break;
	}
	case EntityFactory::STANDARD_HORIZONTAL_RECTANGLE:
	{
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
		_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_2x1x1";	
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(30.f);
		_entity->AddComponent<HealthComponent>(20);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
                _entity->AddComponent<ScoreComponent>().m_score = 2;
		_entity->AddComponent<BlockComponent>();
		_entity->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		_entity->GetComponent<BlockComponent>()->SetDimension(VECTOR2(2, 1));

		auto effect = &_entity->AddComponent<EffectComponent>();
		effect->m_effects.OnAdded = EffectFlags::SCALE_MIN_TO_MAX;
		effect->m_effects.OnCollide = EffectFlags::CHANGE_MODEL;
		break;
	}
	case EntityFactory::PAD:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "pad";
		_entity->AddComponent<MouseInputComponent>();
		_entity->AddComponent<SpawnEntityComponent>(EntityType::BALL, VECTOR3(0, 2, 0));
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<PadCollisionComponent>();
		_entity->AddComponent<AudioComponent>().m_audioPath = "Kettle-Drum-1.wav";
		break;
	case EntityFactory::BALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
		_entity->AddComponent<VelocityComponent>();
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<CollisionStatsComponent>(50.0f, 100.0f, 40.0f, 20.0f);
		_entity->AddComponent<DamageComponent>(10);
		_entity->AddComponent<HealthComponent>(10);
		_entity->AddComponent<LoseLifeComponent>();
		_entity->AddComponent<BallComponent>();
		_entity->AddComponent<EffectComponent>().m_effects.OnAdded = TRAIL;
		break;
	case EntityFactory::WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		//_entity->AddComponent<ModelComponent>().m_modelPath = "wall";
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DeflectionComponent>(50.0f);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wall.wav";
		break;
	case EntityFactory::H_WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		//_entity->AddComponent<ModelComponent>().m_modelPath = "wallH";
		PhysicsSystem::GenerateBody(INVISIBLE_WALL, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<AudioComponent>().m_audioPath = "Wall.wav";
		break;
	case EntityFactory::INVISIBLE_WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		PhysicsSystem::GenerateBody(_entityType, bodyDef, fixDefs);
		_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);
		_entity->AddComponent<DamageComponent>(10);
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
		case EntityFactory::SHATTER:
			_entity->AddComponent<PositionComponent>();
			_entity->AddComponent<RotationComponent>();
			_entity->AddComponent<ScaleComponent>();
			_entity->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1_red";

			_entity->AddComponent<ShatterComponent>().m_explosionState = ShatterComponent::SHATTERING;
		break;
		case EntityFactory::EXPLOSION:
			_entity->AddComponent<PositionComponent>();
			_entity->AddComponent<ExplosionComponent>().m_explosionState = ExplosionComponent::EXPLODING;
		break;
		case EntityFactory::POWERUP:
			_entity->AddComponent<PositionComponent>();
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

			int rnd = (rand() % (3 - 0));

			EntityFactory::GetInstance()->CreateEntity(e, (EntityFactory::EntityType)rnd);
			e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(x, y, 0));
			e->GetComponent<RotationComponent>()->SetRotation(ROTATEYAWPITCHROLL(0, 0, 0));
			BlockComponent* bc = e->GetComponent<BlockComponent>();


			_world->AddEntity(e);
		}




}