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
	b2BodyDef* bodyDef;
	b2FixtureDef* fixDef;
	b2CircleShape* circleShape;
	b2PolygonShape* polygonShape;

	switch (_entityType)
	{
	case EntityFactory::BLOCK:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "box";

		bodyDef = new b2BodyDef();
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		bodyDef->type = b2_staticBody;
		polygonShape->SetAsBox(0.5f, 0.5f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		_entity->AddComponent<CollisionComponent>(fixDef);

		break;
	case EntityFactory::PAD:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "pad";
		//_entity->AddComponent<VelocityComponent>();
		_entity->AddComponent<MouseInputComponent>();

		bodyDef = new b2BodyDef();
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		bodyDef->type = b2_staticBody;
		polygonShape->SetAsBox(2.5f, 0.5f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		_entity->AddComponent<CollisionComponent>(fixDef);


		break;
	case EntityFactory::BALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
		_entity->AddComponent<VelocityComponent>();

		bodyDef = new b2BodyDef();
		fixDef = new b2FixtureDef();
		circleShape = new b2CircleShape();
		bodyDef->type = b2_dynamicBody;
		circleShape->m_p.Set(0, 0);
		circleShape->m_radius = 1.0f;
		fixDef->shape = circleShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		_entity->AddComponent<CollisionComponent>(fixDef);

		break;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "wall";

		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.5f, 10.0f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		_entity->AddComponent<CollisionComponent>(fixDef);
	default:
		break;
	case EntityFactory::PROJECTILE:
		_entity->AddComponent<PositionComponent>();
		_entity->AddComponent<RotationComponent>();
		_entity->AddComponent<ScaleComponent>();
		_entity->AddComponent<ModelComponent>().m_modelPath = "projectile";
		_entity->AddComponent<VelocityComponent>();

		bodyDef = new b2BodyDef();
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		bodyDef->type = b2_staticBody;
		polygonShape->SetAsBox(0.1f, 0.5f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		_entity->AddComponent<CollisionComponent>(fixDef);
		break;
	}
}