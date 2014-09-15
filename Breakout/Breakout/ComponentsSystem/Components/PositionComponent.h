#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "IComponent.h"

struct PositionComponent : Component<PositionComponent>
{
	float m_X;
	float m_Y;
	float m_Z;

	PositionComponent()
	{
		m_X = 0;
		m_Y = 0;
		m_Z = 0;
	}

};

#endif