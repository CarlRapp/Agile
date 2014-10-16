#ifndef EFFECTSYSTEM_H
#define EFFECTSYSTEM_H

#include "ISystem.h"
#include "../Component/EffectComponent.h"

class EffectSystem : public System<EffectSystem>
{
private:

	bool EntityContains(EffectEvents& _entityEvents, EffectFlags _flagToCheck);

	void UpdateEffects(float _dt);

	void OnEntityAdded(Entity* _e);
	void OnEntityRemoved(Entity* _e);

	void OnEveryFrame(Entity* _e, float _dt);
	void OnEverySecond(Entity* _e, float _dt);
	void OnCollision(Entity* _e, float _dt);

	float m_currentTime;
	float m_maxTime;

	EffectEvents m_flags;
	std::map<int, Entity*> m_effects;

public:

	EffectSystem(World* _world);
	~EffectSystem();

	void Update(float _dt);
};

#endif