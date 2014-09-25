#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <Box2D.h>
#include <vector>

#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionComponent : Component<CollisionComponent>
{
private:
	b2Body* m_body;
	b2BodyDef* m_bodyDef;
	b2FixtureDef* m_fixDef;
	bool m_added;
	std::vector<int> m_collidingEntityIds;
public:
	CollisionComponent(b2FixtureDef* _fixDef) : m_added(false), m_fixDef(_fixDef) 
	{
		assert(_fixDef);
	}

	~CollisionComponent()
	{
		if (m_body != 0)
			m_body->GetWorld()->DestroyBody(m_body);
	}

	void CreateBody(b2World* _b2World, b2BodyDef* _bodyDef)
	{
		m_bodyDef = _bodyDef;
		m_body = _b2World->CreateBody(m_bodyDef);
		m_body->CreateFixture(m_fixDef);
		m_added = true;
	}

	void CollidingWith(int _entityId) { m_collidingEntityIds.push_back(_entityId); }
	const std::vector<int>& GetCollisions() const { return m_collidingEntityIds; }
	void ResetCollisions() { m_collidingEntityIds.clear(); }

	b2Body* GetBody() { if (m_added) return m_body; else return 0; }
	bool HasBody(b2Body* _body) { return (_body == m_body); }
	bool IsAdded() { return m_added; }
};

#endif
