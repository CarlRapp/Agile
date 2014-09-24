#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <Box2D.h>

#include "../../stdafx.h"
#include "IComponent.h"
#include "../EntityFactory.h"

struct CollisionComponent : Component<CollisionComponent>
{
public:
	b2Body* m_body;
	b2BodyDef* m_bodyDef;
	b2FixtureDef* m_fixDef;
	bool m_added;
	
	CollisionComponent(b2FixtureDef* _fixDef) : m_added(false), m_fixDef(_fixDef) 
	{
		assert(_fixDef);
	}

	~CollisionComponent()
	{
		if (m_body != 0)
			m_body->GetWorld()->DestroyBody(m_body);
	}
};

#endif
