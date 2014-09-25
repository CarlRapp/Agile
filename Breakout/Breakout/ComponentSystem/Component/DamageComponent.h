#ifndef DAMAGECOMPONENT_H
#define DAMAGECOMPONENT_H

#include "IComponent.h"

struct DamageComponent : Component<DamageComponent>
{
	int m_damage;

	DamageComponent() : m_damage(0) { }
	explicit DamageComponent(int _damage) : m_damage(_damage) { }
};

#endif