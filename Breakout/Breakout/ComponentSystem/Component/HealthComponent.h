#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "IComponent.h"

struct HealthComponent : Component<HealthComponent>
{
	int m_currentHealth;
	int m_maxHealth;

	HealthComponent() : m_currentHealth(0), m_maxHealth(0) { }
	explicit HealthComponent(int _maxHealth) : m_currentHealth(_maxHealth), m_maxHealth(_maxHealth) { }
	HealthComponent(int _currentHealth, int _maxHealth) : m_currentHealth(_currentHealth), m_maxHealth(_maxHealth) { }

	void SetHealth(int _health)
	{
		m_currentHealth = _health;
		m_maxHealth = _health;
	}
};

#endif