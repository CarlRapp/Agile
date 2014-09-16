#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ISystem.h"

class MovementSystem : public System<MovementSystem>
{
private:
public:

	MovementSystem();
	~MovementSystem();

	void Update(double _dt);
};
#endif