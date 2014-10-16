#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#ifdef LINUX
#include <Box2D/Box2D.h>
#else

#include <Box2D.h>
#endif
#include <vector>
#include <map>


#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionContact
{
public:
	b2Fixture* m_fixture;
	b2Fixture* m_otherFixture;
	b2Contact* m_contact;
	int m_otherId;

	CollisionContact(b2Contact* _contact, b2Fixture* _fixture, b2Fixture* _otherFixture, int _otherId) : m_contact(_contact), m_fixture(_fixture), m_otherFixture(_otherFixture), m_otherId(_otherId) { }
	CollisionContact();
	~CollisionContact() 
	{
		m_fixture = 0;
		m_otherFixture = 0;
		m_contact = 0;
	}
};

struct CollisionComponent : Component<CollisionComponent>
{
private:
	b2Body* m_body;
	b2BodyDef* m_bodyDef;
	std::vector<b2FixtureDef*> m_fixDefs;
	std::vector<CollisionContact> m_collisionContacts;
public:

	CollisionComponent(b2BodyDef* _bodyDef, std::vector<b2FixtureDef*> _fixDefs) : m_bodyDef(_bodyDef), m_fixDefs(_fixDefs)
	{
	}

	~CollisionComponent()
	{
		SafeDelete(m_bodyDef);
		if (m_body)
		{
			m_body->GetWorld()->DestroyBody(m_body);
			m_body = 0;
		}
		for (int i = 0; i < m_fixDefs.size(); ++i)
		{
			SafeDelete(m_fixDefs[i]->shape);
			SafeDelete(m_fixDefs[i]);
		}
		m_fixDefs.clear();

	}

	void CreateBody(b2World* _b2World)
	{
		m_body = _b2World->CreateBody(m_bodyDef);
		for (auto it = m_fixDefs.begin(); it != m_fixDefs.end(); ++it)
			m_body->CreateFixture(*it);
	}

	void CollidingWith(CollisionContact _collisionContact) { m_collisionContacts.push_back(_collisionContact); }
	const std::vector<CollisionContact>& GetCollisions() const { return m_collisionContacts; }
	void ResetCollisions() { m_collisionContacts.clear(); }

	b2Body* GetBody() { return m_body; }
	const std::vector<b2FixtureDef*>& GetFixtureDefs() const { return m_fixDefs; }
	b2BodyDef* GetBodyDef() { return m_bodyDef; }
	bool HasBody(b2Body* _body) { return (_body == m_body); }
};

#endif
