#ifndef KILLONTOUCHSYSTEM_H
#define KILLONTOUCHSYSTEM_H

#include "ISystem.h"

class KillOnTouchSystem : public System<KillOnTouchSystem>
{
private:

public:
	KillOnTouchSystem(World* _world);
	~KillOnTouchSystem();


	void Update(float _dt);
};
#endif