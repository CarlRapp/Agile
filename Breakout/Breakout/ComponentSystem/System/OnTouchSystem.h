#ifndef OnTouchSystem_H
#define OnTouchSystem_H

#include "ISystem.h"

class OnTouchSystem : public System<OnTouchSystem>
{
private:

public:
	OnTouchSystem(World* _world);
	~OnTouchSystem();


	void Update(float _dt);
};
#endif