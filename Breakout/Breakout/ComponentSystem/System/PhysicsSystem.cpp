#include "PhysicsSystem.h"
#include "../World.h"

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
	// Simulate worlds physics
	m_b2World->Step(_dt, VELOCITYITERATIONS, POSITIONITERATIONS);

	for (auto it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		auto collision = e->GetComponent<CollisionComponent>();
		auto position = e->GetComponent<PositionComponent>();
		auto rotation = e->GetComponent<RotationComponent>();
		auto velocity = e->GetComponent<VelocityComponent>();
		collision->ResetCollisions();

		if (e->GetState() != Entity::ALIVE)
			continue;

		// Add the body to world if it hasn't been added
		if (!collision->IsAdded())
			if (velocity)
				CreateBody(e, *collision, position->GetPosition(), velocity->m_velocity, 0);
			else
				CreateBody(e, *collision, position->GetPosition(), VECTOR3(0, 0, 0), 0);

		// Update all other components except collision
		b2Body* b2Body = collision->GetBody();
		b2Vec2 b2Pos = b2Body->GetPosition();
		b2Vec2 b2Velocity = b2Body->GetLinearVelocity();
		b2Vec2 b2Rotation = b2Vec2(cos(b2Body->GetAngle()), sin(b2Body->GetAngle()));
		position->SetPosition(VECTOR3(b2Pos.x, b2Pos.y, position->GetPosition().z));
		rotation->SetRotation(MacroRotateYawPitchRoll(rotation->GetRotation().x, rotation->GetRotation().y, b2Body->GetAngle()));
		if (velocity) 
			velocity->m_velocity = VECTOR3(b2Velocity.x, b2Velocity.y, velocity->m_velocity.z);
	}

	// Do collisions checks
	for (b2Contact* contact = m_b2World->GetContactList(); contact; contact = contact->GetNext())
	{
		if (!contact->IsTouching())
			continue;

		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		Entity* entityA = 0;
		Entity* entityB = 0;
		for (auto it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
		{
			Entity* e = it->second;
			if (!entityA && e->GetComponent<CollisionComponent>()->HasBody(fixtureA->GetBody()))
				entityA = e;
			else if (!entityB && e->GetComponent<CollisionComponent>()->HasBody(fixtureB->GetBody()))
				entityB = e;
			if (entityA && entityB)
				break;
		}
		b2WorldManifold* manifold = new b2WorldManifold();
		contact->GetWorldManifold(manifold);
		if (entityA && entityB)
		{
			entityA->GetComponent<CollisionComponent>()->CollidingWith(entityB->GetId(), *manifold);
			entityB->GetComponent<CollisionComponent>()->CollidingWith(entityA->GetId(), *manifold);
		}
	}
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
		fixDef->friction = 0.0f;
		fixDef->filter.categoryBits = CollisionCategory::BLOCK;
		break;
	case EntityFactory::PAD:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(2.5f, 0.5f);
		fixDef->shape = polygonShape;
		fixDef->friction = 1.0f;
		fixDef->filter.categoryBits = CollisionCategory::PAD;
		break;
	case EntityFactory::BALL:
		fixDef = new b2FixtureDef();
		circleShape = new b2CircleShape();
		circleShape->m_p.Set(0, 0);
		circleShape->m_radius = 1.0f;
		fixDef->shape = circleShape;
		fixDef->density = 5000.0f;
		fixDef->friction = 1.0f;
		fixDef->restitution = 1.0f;
		fixDef->filter.categoryBits = CollisionCategory::BALL;
		break;
	case EntityFactory::PROJECTILE:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.1f, 1.0f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.0f;
		fixDef->filter.categoryBits = CollisionCategory::WALL;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.5f, 30.0f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.0f;
		fixDef->filter.categoryBits = CollisionCategory::WALL;
		break;
	case EntityFactory::INVISIBLE_WALL:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(94.f, 0.f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.0f;
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