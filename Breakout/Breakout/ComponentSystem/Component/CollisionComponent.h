#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <Box2D.h>
#include <vector>
#include <map>

#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionComponent : Component<CollisionComponent>
{
private:
	b2Body* m_body;
	b2BodyDef* m_bodyDef;
	b2FixtureDef* m_fixDef;
	std::vector<int> m_collidingEntityIds;
	std::map<int, b2WorldManifold*> m_collidingManifolds;
public:
	CollisionComponent(b2BodyDef* _bodyDef, b2FixtureDef* _fixDef) : m_bodyDef(_bodyDef), m_fixDef(_fixDef) 
	{
		assert(_bodyDef && _fixDef);
	}

	~CollisionComponent()
	{
		if (m_body != 0)
			m_body->GetWorld()->DestroyBody(m_body);
	}

	void CreateBody(b2World* _b2World)
	{
		m_body = _b2World->CreateBody(m_bodyDef);
		m_body->CreateFixture(m_fixDef);
	}

	void CollidingWith(int _entityId, b2WorldManifold& _manifold) { m_collidingEntityIds.push_back(_entityId); m_collidingManifolds[_entityId] = &_manifold; }
	const std::vector<int>& GetCollisions() const { return m_collidingEntityIds; }
	const b2WorldManifold* GetManifold(int _entityId) { if (m_collidingManifolds.find(_entityId) != m_collidingManifolds.end()) return m_collidingManifolds[_entityId]; return 0; }
	void ResetCollisions() { m_collidingEntityIds.clear(); m_collidingManifolds.clear(); }

	b2Body* GetBody() { return m_body; }
	bool HasBody(b2Body* _body) { return (_body == m_body); }
};

#endif
