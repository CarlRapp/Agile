#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "IComponent.h"

struct HealthComponent : Component<HealthComponent>
{
	int m_currentHealth;
	int m_maxHealth;

	HealthComponent()
	{
		m_currentHealth = 0;
		m_maxHealth = 0;
	}

};

#endif