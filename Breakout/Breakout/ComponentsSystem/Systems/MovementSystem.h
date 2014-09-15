#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ISystem.h"

struct MovementSystem : System<MovementSystem>
{
	MovementSystem();

	void Update(double _dt);
};

#endif