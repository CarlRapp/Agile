#ifndef EFFECTSYSTEM_H
#define EFFECTSYSTEM_H

#include "ISystem.h"

class EffectSystem : public System<EffectSystem>
{
private:

	void UpdateComponents(Entity* _entity, float _dt);

public:

	EffectSystem(World* _world);
	~EffectSystem();

	void Update(float _dt);
};

#endif