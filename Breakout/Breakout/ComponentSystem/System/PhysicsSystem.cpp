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

		if (!collision->m_added)
			CreateBody(e, *collision, position->GetPosition(), atan2(rotation->m_rotation.x, rotation->m_rotation.y));

		b2Vec2 b2Pos = collision->m_body->GetPosition();
		b2Vec2 b2Velocity = collision->m_body->GetLinearVelocity();
		b2Vec2 b2Rotation = b2Vec2(cos(collision->m_body->GetAngle()), sin(collision->m_body->GetAngle()));

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

void PhysicsSystem::CreateBody(Entity* _entity, CollisionComponent& _collision, const VECTOR3& _position, float rotation)
{
	_collision.m_bodyDef = new b2BodyDef();
	_collision.m_bodyDef->position.Set(_position.x, _position.y);
	_collision.m_bodyDef->angle = rotation;
	if (_entity->HasComponent<VelocityComponent>())
		_collision.m_bodyDef->type = b2_dynamicBody;
	else
		_collision.m_bodyDef->type = b2_staticBody;
	_collision.m_body = m_b2World->CreateBody(_collision.m_bodyDef);
	_collision.m_body->CreateFixture(_collision.m_fixDef);

	_collision.m_added = true;
}