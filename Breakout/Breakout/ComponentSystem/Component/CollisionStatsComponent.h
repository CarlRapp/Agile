#ifndef COLLISIONSTATSCOMPONENT_H
#define COLLISIONSTATSCOMPONENT_H

#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionStatsComponent : Component<CollisionStatsComponent>
{
private:
	float m_minSpeed, m_maxSpeed, m_maxDampingSpeed, m_dampingAcceleration;
public:
	CollisionStatsComponent() : m_minSpeed(0.0f), m_maxSpeed(0.0f), m_maxDampingSpeed(0.0f), m_dampingAcceleration(0.0f) { }
	CollisionStatsComponent(float _minSpeed, float _maxSpeed, float _maxDampingSpeed, float _dampingAcceleration) 
		: m_minSpeed(_minSpeed), m_maxSpeed(_maxSpeed), m_maxDampingSpeed(_maxDampingSpeed), m_dampingAcceleration(_dampingAcceleration) { }
	~CollisionStatsComponent() { }

	float GetMinSpeed() const { return m_minSpeed; }
	float GetMaxSpeed() const { return m_maxSpeed; }
	float GetMaxDampingSpeed() const { return m_maxDampingSpeed; }
	float GetDampingAcceleration() const { return m_dampingAcceleration; }
	void SetMinSpeed(const float _minSpeed) { m_minSpeed = _minSpeed; }
	void SetMaxSpeed(const float _maxSpeed) { m_maxSpeed = _maxSpeed; }
	void SetMaxDampingSpeed(const float _maxDampingSpeed) { m_maxDampingSpeed = _maxDampingSpeed; }
	void SetDampingAcceleration(const float _dampingAcceleration) { m_dampingAcceleration = _dampingAcceleration; }
};

#endif
