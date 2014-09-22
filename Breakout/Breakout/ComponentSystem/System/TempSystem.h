#ifndef TEMPSYSTEM_H
#define TEMPSYSTEM_H

#include "ISystem.h"

class TempSystem : public System<TempSystem>
{
private:
public:

	TempSystem(World* _world);
	~TempSystem();

	void Update(float _dt);
};
#endif