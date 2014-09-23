#ifndef _PHYSICSSYSTEM_H_
#define _PHYSICSSYSTEM_H_

#include <vector>
#include <Box2D.h>

#include "ISystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/CollisionComponent.h"

class PhysicsSystem : public System<PhysicsSystem>
{
private:
	const int VELOCITYITERATIONS = 8;
	const int POSITIONITERATIONS = 2;
	const b2Vec2 DEFAULTGRAVITY = b2Vec2(0, -10.0f);
	
	b2World* m_b2World;
	
	void CreateBody(Entity* entity, CollisionComponent& _collision, const VECTOR3& _position, float rotation);

public:
	PhysicsSystem(World* _world);
	~PhysicsSystem();

	void Update(float _dt);

	void SetGravity(const b2Vec2& _gravity) { m_b2World->SetGravity(_gravity); }

	b2World& GetB2World() { return *m_b2World; }
};

#endif