#ifndef COLLISIONSTATSCOMPONENT_H
#define COLLISIONSTATSCOMPONENT_H

#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionStatsComponent : Component<CollisionStatsComponent>
{
private:
	float m_minSpeed, m_maxSpeed;
public:
	CollisionStatsComponent() : m_minSpeed(0), m_maxSpeed(0) { }
	CollisionStatsComponent(float _minSpeed, float _maxSpeed) : m_minSpeed(_minSpeed), m_maxSpeed(_maxSpeed) { }
	~CollisionStatsComponent() { }

	float GetMinSpeed() const { return m_minSpeed; }
	float GetMaxSpeed() const { return m_maxSpeed; }
	void SetMinSpeed(const float _minSpeed) { m_minSpeed = _minSpeed; }
	void SetMaxSpeed(const float _maxSpeed) { m_maxSpeed = _maxSpeed; }
};

#endif
