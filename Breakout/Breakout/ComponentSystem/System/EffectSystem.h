#ifndef EFFECTSYSTEM_H
#define EFFECTSYSTEM_H

#include "ISystem.h"
#include "../Component/EffectComponent.h"

class EffectSystem : public System<EffectSystem>
{
private:

	void UpdateComponents(Entity* _entity, float _dt);

	void OnEntityAdded(Entity* _e);
	void OnEveryFrame(Entity* _e, float _dt);
	void OnEverySecond(Entity* _e, float _dt);
	void OnCollision(Entity* _e, float _dt);
	void OnRemove(Entity* _e, float _dt);

	float m_currentTime;
	float m_maxTime;

	EffectEvents m_flags;

public:

	EffectSystem(World* _world);
	~EffectSystem();

	void Update(float _dt);
};

#endif