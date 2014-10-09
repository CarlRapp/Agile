#ifndef TNTCOMPONENT_H
#define TNTCOMPONENT_H

#include "IComponent.h"

struct TNTComponent : Component<TNTComponent>
{
	int m_radius;

	TNTComponent() : m_radius(1) { }
	void SetRadius(int _radius){ m_radius = _radius; }
};

#endif