#ifndef DAMAGECOMPONENT_H
#define DAMAGECOMPONENT_H

#include "IComponent.h"

struct DamageComponent : Component<DamageComponent>
{
	int m_currentHealth;
	int m_maxHealth;

	DamageComponent()
	{
		m_currentHealth = 0;
		m_maxHealth = 0;
	}

};

#endif