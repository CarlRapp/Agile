#ifndef LIFECOMPONENT_H
#define LIFECOMPONENT_H

#include "IComponent.h"

struct LifeComponent : Component<LifeComponent>
{
	int m_maxNoLifes;
	int m_noLifes;

	LifeComponent()
	{
		m_maxNoLifes = 10;
		m_noLifes = 3;
	}

};

#endif