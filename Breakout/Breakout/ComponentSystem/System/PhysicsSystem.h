#ifndef _PHYSICSSYSTEM_H_
#define _PHYSICSSYSTEM_H_

#include <vector>
#include <Box2D/Box2D.h>

#include "ISystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/CollisionComponent.h"
#include "../EntityFactory.h"

enum CollisionCategory {
	WALL = 0x0001,
	INVISIBLEWALL = 0x0002,
	PAD = 0x0004,
	BLOCK = 0x0008,
	BALL = 0x0010,
	POWERUP = 0x0020
};

class PhysicsSystem : public System<PhysicsSystem>
{
private:
	const int VELOCITYITERATIONS = 8;
	const int POSITIONITERATIONS = 2;
	const b2Vec2 DEFAULTGRAVITY = b2Vec2(0, 0.0f);
	
	b2World* m_b2World;
	
public:
	explicit PhysicsSystem(World* _world);
	~PhysicsSystem();

	void Update(float _dt);
	void OnEntityAdded(Entity* _entity);
	void OnEntityRemoved(Entity* _entity);

	void SetGravity(const b2Vec2& _gravity) { m_b2World->SetGravity(_gravity); }

	b2World& GetB2World() { return *m_b2World; }

	// Get the right fixture definition for the game object
	static void GenerateBody(unsigned int _entityType, b2BodyDef* _b2BodyDef, vector<b2FixtureDef*>& _b2FixtureDefs);
};


#endif