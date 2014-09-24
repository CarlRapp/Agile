#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ISystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/MouseInputComponent.h"
#include "../Component/CollisionComponent.h"
#include "../System/PhysicsSystem.h"

class MovementSystem : public System<MovementSystem>
{
private:
public:

	MovementSystem(World* _world);
	~MovementSystem();

	void Update(float _dt);
};
#endif