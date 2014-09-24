#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(World* _world) 
: Base(ComponentFilter().Requires<PositionComponent, RotationComponent, CollisionComponent>(), _world)
{
	m_b2World = new b2World(DEFAULTGRAVITY);
}


PhysicsSystem::~PhysicsSystem()
{
	delete m_b2World;
}

void PhysicsSystem::Update(float _dt)
{
	m_b2World->Step(_dt, VELOCITYITERATIONS, POSITIONITERATIONS);
	for (auto it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;

		if (e->GetState() != Entity::ALIVE)
			continue;

		auto position = e->GetComponent<PositionComponent>();
		auto rotation = e->GetComponent<RotationComponent>();
		auto velocity = e->GetComponent<VelocityComponent>();
		auto collision = e->GetComponent<CollisionComponent>();

		if (!collision->IsAdded())
			if (velocity)
				CreateBody(e, *collision, position->GetPosition(), velocity->m_velocity, atan2(rotation->m_rotation.x, rotation->m_rotation.y));
			else
				CreateBody(e, *collision, position->GetPosition(), VECTOR3(0,0,0), atan2(rotation->m_rotation.x, rotation->m_rotation.y));
		b2Body* body = collision->GetBody();
		b2Vec2 b2Pos = body->GetPosition();
		b2Vec2 b2Velocity = body->GetLinearVelocity();
		b2Vec2 b2Rotation = b2Vec2(cos(body->GetAngle()), sin(body->GetAngle()));

		position->SetPosition(VECTOR3(b2Pos.x, b2Pos.y, position->GetPosition().z));
		rotation->m_rotation = VECTOR3(b2Rotation.x, b2Rotation.y, rotation->m_rotation.z);
		if (velocity) 
			velocity->m_velocity = VECTOR3(b2Velocity.x, b2Velocity.y, velocity->m_velocity.z);
	}
	/*
	if (m_b2World->GetBodyList() != 0)
	{
		b2Vec2 position = m_b2World->GetBodyList()->GetPosition();
		float32 angle = m_b2World->GetBodyList()->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}*/
}

void PhysicsSystem::CreateBody(Entity* _entity, CollisionComponent& _collision, const VECTOR3& _position, const VECTOR3& _velocity, float rotation)
{
	b2BodyDef* bodyDef = new b2BodyDef();
	bodyDef->position.Set(_position.x, _position.y);
	bodyDef->linearVelocity = b2Vec2(_velocity.x, _velocity.y);
	bodyDef->angle = rotation;
	if (_entity->HasComponent<MouseInputComponent>())
		bodyDef->type = b2_kinematicBody;
	else if (_entity->HasComponent<VelocityComponent>())
		bodyDef->type = b2_dynamicBody;
	else
		bodyDef->type = b2_staticBody;

	_collision.CreateBody(m_b2World, bodyDef);
}

b2FixtureDef* PhysicsSystem::GenerateFixtureDefinition(unsigned int _entityType)
{
	b2FixtureDef* fixDef = 0;
	b2PolygonShape* polygonShape;
	b2CircleShape* circleShape;

	switch (_entityType)
	{
	case EntityFactory::BLOCK:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.5f, 0.5f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		fixDef->filter.categoryBits = CollisionCategory::BLOCK;
		break;
	case EntityFactory::PAD:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(2.5f, 0.5f);
		fixDef->shape = polygonShape;
		fixDef->friction = 0.0f;
		fixDef->filter.categoryBits = CollisionCategory::PAD;
		break;
	case EntityFactory::BALL:
		fixDef = new b2FixtureDef();
		circleShape = new b2CircleShape();
		circleShape->m_p.Set(0, 0);
		circleShape->m_radius = 1.0f;
		fixDef->shape = circleShape;
		fixDef->density = 5000.0f;
		fixDef->friction = 50.0f;
		fixDef->filter.categoryBits = CollisionCategory::BALL;
		break;
	case EntityFactory::PROJECTILE:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.1f, 1.0f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		fixDef->filter.categoryBits = CollisionCategory::BALL;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.5f, 10.0f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.3f;
		fixDef->filter.categoryBits = CollisionCategory::WALL;
	default:
		break;
	}

	return fixDef;
}
/*
b2FixtureDef* PhysicsSystem::GenerateFixtureDefinition(EntityFactory::EntityType _entityType, float size, float density, float friction, float restitution)
{
	return 0;
}
b2FixtureDef* PhysicsSystem::GenerateFixtureDefinition(EntityFactory::EntityType _entityType, float size, float density, float friction, float restitution, uint16 maskBits)
{
	return 0;
}
b2FixtureDef* PhysicsSystem::GenerateFixtureDefinition(EntityFactory::EntityType _entityType, uint16 maskBits)
{
	return 0;
}
*/