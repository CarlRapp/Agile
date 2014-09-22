#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <Box2D.h>

#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionComponent : Component<CollisionComponent>
{
private:
	CollisionComponent();

public:
	b2Body* m_body;

	CollisionComponent(b2World& _world, const b2BodyDef& _bodyDef, const b2FixtureDef& _fixtureDef)
	{
		m_body = _world.CreateBody(&_bodyDef);
		m_body->CreateFixture(&_fixtureDef);
	}

	CollisionComponent(b2World& _world, const b2BodyDef& _bodyDef, const b2PolygonShape& _shape, float _density)
	{
		m_body = _world.CreateBody(&_bodyDef);
		m_body->CreateFixture(&_shape, _density);
	}

	~CollisionComponent()
	{
		m_body->GetWorld()->DestroyBody(m_body);
	}
};

#endif
