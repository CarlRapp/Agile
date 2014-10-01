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
	// Update velocity manually
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
			if (b2Body->GetLinearVelocity().y <= 0.5f && b2Body->GetLinearVelocity().y >= -0.5f)
				b2Body->SetLinearVelocity(b2Vec2(b2Body->GetLinearVelocity().x, -3.0f));

			// Speed cant be 0
			float speed = b2Body->GetLinearVelocity().Length();
			if (speed == 0)
				continue;

			// Set speed between min/max
			if (speed < stats->GetMinSpeed())
			{
				b2Body->SetLinearVelocity(b2Vec2((b2Body->GetLinearVelocity().x / speed) * stats->GetMinSpeed(), (b2Body->GetLinearVelocity().y / speed) * stats->GetMinSpeed()));
			}
			if (speed > stats->GetMaxSpeed())
			{
				b2Body->SetLinearVelocity(b2Vec2((b2Body->GetLinearVelocity().x / speed) * stats->GetMaxSpeed(), (b2Body->GetLinearVelocity().y / speed) * stats->GetMaxSpeed()));
			}
			
			// Deaccelerate
			if (speed > stats->GetMaxDampingSpeed())
			{
				float newSpeed = speed - (stats->GetDampingAcceleration() * _dt);
				if (speed < stats->GetMaxDampingSpeed())
					newSpeed = stats->GetMaxDampingSpeed();
				b2Body->SetLinearVelocity(b2Vec2((b2Body->GetLinearVelocity().x / speed) * newSpeed, (b2Body->GetLinearVelocity().y / speed) * newSpeed));
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
			rotation->SetRotation(VECTOR3(rotation->GetRotation().x, rotation->GetRotation().y, b2Body->GetAngle()));
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
		CollisionContact collisionContact = CollisionContact(contact, fixtureA, fixtureB, entityB->GetId());
		entityA->GetComponent<CollisionComponent>()->CollidingWith(collisionContact);
		collisionContact = CollisionContact(contact, fixtureB, fixtureA, entityA->GetId());
		entityB->GetComponent<CollisionComponent>()->CollidingWith(collisionContact);
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
		rotation = rotComponent->GetRotation().z;
	
	collision->GetBody()->SetTransform(b2Vec2(position.x, position.y), rotation);
	collision->GetBody()->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));

	return true;
}

void PhysicsSystem::GenerateBody(unsigned int _entityType, b2BodyDef* _b2BodyDef, vector<b2FixtureDef*>& _b2FixtureDefs)
{
	b2PolygonShape* polygonShape;
	b2CircleShape* circleShape;
	b2FixtureDef* fixDef;

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
		_b2FixtureDefs.push_back(fixDef);
		_b2BodyDef->type = b2_staticBody;
		break;
	case EntityFactory::PAD:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(2.5f, 0.5f);
		fixDef->shape = polygonShape;
		fixDef->friction = .5f;
		fixDef->filter.categoryBits = CollisionCategory::PAD;
		_b2FixtureDefs.push_back(fixDef);
		_b2BodyDef->type = b2_kinematicBody;
		break;
	case EntityFactory::BALL:
		fixDef = new b2FixtureDef();
		circleShape = new b2CircleShape();
		circleShape->m_p.Set(0, 0);
		circleShape->m_radius = 1.0f;
		fixDef->shape = circleShape;
		fixDef->density = 1.0f;
		fixDef->friction = 1.0f;
		fixDef->restitution = 1.0f;
		fixDef->filter.categoryBits = CollisionCategory::BALL;
		_b2FixtureDefs.push_back(fixDef);
		_b2BodyDef->type = b2_dynamicBody;
		_b2BodyDef->fixedRotation = true;
		break;
	case EntityFactory::PROJECTILE:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.1f, 1.0f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.0f;
		fixDef->filter.categoryBits = CollisionCategory::BALL;
		_b2FixtureDefs.push_back(fixDef);
		_b2BodyDef->type = b2_dynamicBody;
	case EntityFactory::POWERUP:
		break;
	case EntityFactory::WALL:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(0.5f, 15.0f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.0f;
		fixDef->filter.categoryBits = CollisionCategory::WALL;
		_b2FixtureDefs.push_back(fixDef);
		_b2BodyDef->type = b2_staticBody;
		break;
	case EntityFactory::INVISIBLE_WALL:
		fixDef = new b2FixtureDef();
		polygonShape = new b2PolygonShape();
		polygonShape->SetAsBox(24.f, 0.f);
		fixDef->shape = polygonShape;
		fixDef->density = 1.0f;
		fixDef->friction = 0.0f;
		fixDef->filter.categoryBits = CollisionCategory::INVISIBLEWALL;
		_b2FixtureDefs.push_back(fixDef);
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