#include "PhysicsSystem.h"
#include "../World.h"

PhysicsSystem::PhysicsSystem(World* _world) 
: Base(ComponentFilter().Requires<CollisionComponent>(), _world)
{
	m_b2World = new b2World(DEFAULTGRAVITY);
}


PhysicsSystem::~PhysicsSystem()
{
	delete m_b2World;
}

void PhysicsSystem::Update(float _dt)
{
	m_b2World->SetGravity(b2Vec2(0, -15));
	// Set speed between min/max
	for (auto it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		auto stats = e->GetComponent<CollisionStatsComponent>();
		auto velocity = e->GetComponent<VelocityComponent>();
		b2Body* b2Body = collision->GetBody();

		if (velocity && stats)
		{
			float speed = b2Body->GetLinearVelocity().Length();
			if (speed < stats->GetMinSpeed())
			{
				b2Body->SetLinearVelocity(b2Vec2((b2Body->GetLinearVelocity().x / speed) * stats->GetMinSpeed(), (b2Body->GetLinearVelocity().y / speed) * stats->GetMinSpeed()));
			}
			if (speed > stats->GetMaxSpeed())
			{
				b2Body->SetLinearVelocity(b2Vec2((b2Body->GetLinearVelocity().x / speed) * stats->GetMaxSpeed(), (b2Body->GetLinearVelocity().y / speed) * stats->GetMaxSpeed()));
			}
		}
	}

	// Simulate worlds physics
	m_b2World->Step(_dt, VELOCITYITERATIONS, POSITIONITERATIONS);

	// Update position, velocity and rotation component
	for (auto it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;

		if (e->GetState() != Entity::ALIVE)
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		auto position = e->GetComponent<PositionComponent>();
		auto velocity = e->GetComponent<VelocityComponent>();
		auto rotation = e->GetComponent<RotationComponent>();
		b2Body* b2Body = collision->GetBody();

		collision->ResetCollisions();

		// Update all other components except collision
		if (position)
		{
			b2Vec2 b2Pos = b2Body->GetPosition();
			position->SetPosition(VECTOR3(b2Pos.x, b2Pos.y, position->GetPosition().z));
		}
		if (velocity)
		{
			b2Vec2 b2Velocity = b2Body->GetLinearVelocity();
			velocity->m_velocity = VECTOR3(b2Velocity.x, b2Velocity.y, velocity->m_velocity.z);
		}
		if (rotation)
		{
			b2Vec2 b2Rotation = b2Vec2(cos(b2Body->GetAngle()), sin(b2Body->GetAngle()));
			rotation->m_rotation = VECTOR3(b2Rotation.x, b2Rotation.y, rotation->m_rotation.z);
		}
			
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
			if (e->GetState() != Entity::ALIVE)
				continue;
			if (!entityA && e->GetComponent<CollisionComponent>()->HasBody(fixtureA->GetBody()))
				entityA = e;
			else if (!entityB && e->GetComponent<CollisionComponent>()->HasBody(fixtureB->GetBody()))
				entityB = e;
			if (entityA && entityB)
				break;
		}
		b2WorldManifold* manifold = new b2WorldManifold();
		contact->GetWorldManifold(manifold);
		entityA->GetComponent<CollisionComponent>()->CollidingWith(entityB->GetId(), *manifold);
		entityB->GetComponent<CollisionComponent>()->CollidingWith(entityA->GetId(), *manifold);
	}
}

bool PhysicsSystem::Add(Entity* _entity)
{
	if (!Base::Add(_entity))
		return false;

	auto collision = _entity->GetComponent<CollisionComponent>();
	auto posComponent = _entity->GetComponent<PositionComponent>();
	auto velComponent = _entity->GetComponent<VelocityComponent>();
	auto rotComponent = _entity->GetComponent<RotationComponent>();

	collision->CreateBody(m_b2World);

	b2Vec2 position, velocity;
	float rotation;
	if (posComponent)
		position = b2Vec2(posComponent->GetPosition().x, posComponent->GetPosition().y);
	if (velComponent)
		velocity = b2Vec2(velComponent->m_velocity.x, velComponent->m_velocity.y);
	if (rotComponent)
		rotation = atan2(rotComponent->m_rotation.x, rotComponent->m_rotation.y);
	
	collision->GetBody()->SetTransform(b2Vec2(position.x, position.y), rotation);
	collision->GetBody()->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));

	return true;
}

void PhysicsSystem::GenerateBody(unsigned int _entityType, b2BodyDef* _b2BodyDef, b2FixtureDef* _b2FixtureDef)
{
	b2PolygonShape* polygonShape;
	b2CircleShape* circleShape;

	switch (_entityType)
	{
	case EntityFactory::BLOCK:
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.5f, 0.5f);
		_b2FixtureDef->shape = polygonShape;
		_b2FixtureDef->density = 1.0f;
		_b2FixtureDef->friction = 0.0f;
		_b2FixtureDef->filter.categoryBits = CollisionCategory::BLOCK;
		_b2BodyDef->type = b2_staticBody;
		break;
	case EntityFactory::PAD:
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(2.5f, 0.5f);
		_b2FixtureDef->shape = polygonShape;
		_b2FixtureDef->friction = 1.0f;
		_b2FixtureDef->filter.categoryBits = CollisionCategory::PAD;
		_b2BodyDef->type = b2_kinematicBody;
		break;
	case EntityFactory::BALL:
		circleShape = new b2CircleShape();
		circleShape->m_p.Set(0, 0);
		circleShape->m_radius = 1.0f;
		_b2FixtureDef->shape = circleShape;
		_b2FixtureDef->density = 5000.0f;
		_b2FixtureDef->friction = 1.0f;
		_b2FixtureDef->restitution = 1.0f;
		_b2FixtureDef->filter.categoryBits = CollisionCategory::BALL;
		_b2BodyDef->type = b2_dynamicBody;
		_b2BodyDef->fixedRotation = true;
		break;
	case EntityFactory::PROJECTILE:
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.1f, 1.0f);
		_b2FixtureDef->shape = polygonShape;
		_b2FixtureDef->density = 1.0f;
		_b2FixtureDef->friction = 0.0f;
		_b2FixtureDef->filter.categoryBits = CollisionCategory::BALL;
		_b2BodyDef->type = b2_dynamicBody;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.5f, 15.0f);
		_b2FixtureDef->shape = polygonShape;
		_b2FixtureDef->density = 1.0f;
		_b2FixtureDef->friction = 0.0f;
		_b2FixtureDef->filter.categoryBits = CollisionCategory::WALL;
		_b2BodyDef->type = b2_staticBody;
		break;
	case EntityFactory::INVISIBLE_WALL:
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(24.f, 0.f);
		_b2FixtureDef->shape = polygonShape;
		_b2FixtureDef->density = 1.0f;
		_b2FixtureDef->friction = 0.0f;
		_b2FixtureDef->filter.categoryBits = CollisionCategory::INVISIBLEWALL;
		_b2BodyDef->type = b2_staticBody;
	default:
		break;
	}
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