#ifndef _PHYSICSSYSTEM_H_
#define _PHYSICSSYSTEM_H_

#include <vector>
#include <Box2D.h>

#include "ISystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/SphereComponent.h"
#include "../Component/BoxComponent.h"
#include "../../Game/Collision/CollisionMath.h"

class PhysicsSystem : public System<PhysicsSystem>
{
private:
	const int VELOCITYITERATIONS = 8;
	const int POSITIONITERATIONS = 2;
	

	b2World* m_world;
	

public:
	PhysicsSystem();
	explicit PhysicsSystem(const b2Vec2& _gravity);
	~PhysicsSystem();

	void Update(double _dt);

	void SetGravity(b2Vec2& _vector) { m_world->SetGravity(_vector); }
};

#endif